#pragma once

#include <aether/aether.h>
#include <queue>
#include <algorithm>

#include "constants.h"

enum EntityType
{
    Parisher,
    Bank,
    Virgen,
    Cherub,
    Cura,
    Door
};

struct Entity
{
    typedef std::shared_ptr<Entity> SharedPtr;
    aether::graphics::Texture texture;
    aether::math::Vec2i position = {0, 0};
    int sellCost = 5;
    EntityType etype;
    float ascensionBar = 10;
    int tool_widget_index = -1;
    bool is_bad = false;
    aether::graphics::Texture bad_texture;
    float multiplier = 1.f;
};

struct ToolWidget
{
    aether::graphics::Texture tex;
    EntityType createdBuildingType;
    int basePrice;
    float efficiencyMultiplier;
    float priceMultiplier;
    int currentItems;

    int price()
    {
        int exp = currentItems - 1;
        exp = (exp >= 0) ? exp : 0;
        return int(double(basePrice) * pow(priceMultiplier, double(currentItems-1)));
    }
};

typedef aether::math::Matrix2D<uint8_t> TileMap;
typedef aether::math::Matrix2D<Entity::SharedPtr> EntityMap;

class Scene
{
public:

    static constexpr size_t Width = 16;
    static constexpr size_t Height = 15;
    static constexpr size_t TileSize = 16;
    static constexpr size_t YOffset = TileSize * 4;

    enum Status
    {
        Running,
        LostApocalypsisFullBar,
        LostTooMuchParishers,
        ManuallyRestarted,
        Won
    };

    Scene(aether::graphics::Spritesheet* sheet)
        : m_backgroundMap(Width, Height),
          m_frontEntitiesMap(Width, Height),
          m_buildingEntitiesMap(Width, Height),
          m_sheet(sheet)
    {
        m_backgroundMap.data({
                                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                                2,2,2,2,2,2,2,0,0,2,2,2,2,2,2,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
                                2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        });
        addDoor(7, 1);
        addDoor(8, 1);
        m_widgets.push_back({sheet->getFrame(1), EntityType::Bank, Constants::BankCost, 0, Constants::BankCostMultiplier, 0});
        m_widgets.push_back({sheet->getFrame(7), EntityType::Virgen, Constants::VirgenRocioCost, Constants::VirgenRocioPrayEfficiencyMultiplier, Constants::VirgenRocioCostMultiplier, 0});
        m_widgets.push_back({sheet->getFrame(10), EntityType::Cura, Constants::PriestCost, Constants::PriestPrayEfficiencyMultiplier, Constants::PriestCostMultiplier, 0});
        m_widgets.push_back({sheet->getFrame(8), EntityType::Cherub, Constants::CherubCost, Constants::CherubPrayEfficiencyMultiplier, Constants::CherubCostMultiplier, 0});
        m_selectedWidgetIndex = 0;
        recomputePrayMultiplier();
    }

    ~Scene()
    {

    }

    void render()
    {
        for( size_t i = 0; i < Height; i++ )
        {
            for( size_t j = 0; j < Width; j++ )
            {
                int xpos, ypos;
                xpos = j * TileSize;
                ypos = YOffset + i * TileSize;
                aether::graphics::Texture bg_tile = m_sheet->getFrame(m_backgroundMap.get(j, i));
                bg_tile.draw(j * TileSize, YOffset + i * TileSize);
                auto building = m_buildingEntitiesMap.get(j, i);
                if( building != nullptr )
                {
                    if( false == building->is_bad )
                    {
                        building->texture.draw(xpos, ypos);
                    }
                    else
                    {
                        building->bad_texture.draw(xpos, ypos);
                    }
                }
            }
        }

        for( auto entity : m_sitParishers )
        {
            int xpos, ypos;
            xpos = entity->position.x() * TileSize;
            ypos = YOffset + entity->position.y() * TileSize;
            if( entity != nullptr )
            {
                entity->texture.draw(xpos, ypos);
                int max_barra = 16;
                int current_value = entity->ascensionBar;
                int max_value = 100;
                int current_barra = max_barra * current_value / max_value;
                aether::graphics::draw_filled_rectangle(
                            xpos, ypos,
                            xpos + current_barra, ypos + 2,
                            aether::graphics::Color(0, 255, 255)
                            );
            }
        }

        for( size_t i = 0; i < m_widgets.size(); i++ )
        {
            auto& w = m_widgets[i];
            w.tex.draw(i * 16, 48);
        }

        m_sheet->getFrame(3).draw(m_selectedWidgetIndex * 16, 48);

        auto& ms = aether::core::_get_mouse_state();
        auto v = unproject(ms.x, ms.y);
        m_sheet->getFrame(3).draw(v.x() * 16, YOffset + (v.y()) * 16);

        int i = 0;
        for( auto& e : m_parisherQueue )
        {
            e->texture.draw(i * 16, 16 * 4);
            i++;
        }


        float vv = 125 * m_apocalypsisLevel / 100;
        float xbar = 120;
        float ybar = 309;
        aether::graphics::draw_filled_rectangle(
                    xbar, ybar,
                    xbar + vv, ybar + 8,
                    aether::graphics::Color(255, 0, 0)
                    );


    }

    void enqueueParisher()
    {
        int num_parishers = 1;
        if( m_apocalypsisLevel > 25 )
        {
            num_parishers = 1;
        }
        else if( m_apocalypsisLevel > 50 )
        {
            num_parishers = 2;
        }
        else if( m_apocalypsisLevel > 75 )
        {
            num_parishers = 2;
        }
        else if( m_apocalypsisLevel > 80 )
        {
            num_parishers = 2;
        }
        else if( m_apocalypsisLevel > 90 )
        {
            num_parishers = 2;
        }
        while( num_parishers > 0 )
        {
            Entity::SharedPtr e(new Entity);
            e->texture = m_sheet->getFrame(9);
            e->etype = EntityType::Parisher;
            m_parisherQueue.push_back(e);
            if( m_parisherQueue.size() > 16 )
            {
                m_sceneStatus = Status::LostTooMuchParishers;
            }
            num_parishers--;
        }
    }

    void addDoor( int cx, int cy )
    {
        Entity::SharedPtr e( new Entity );
        e->texture = m_sheet->getFrame(6);
        m_frontEntitiesMap.set(cx, cy, e);
        e->etype = EntityType::Door;
    }

    void tryCreateBuildingAt( int cx, int cy )
    {
        if( m_buildingEntitiesMap.get(cx, cy) == nullptr )
        {
            auto& sw = selectedWidget();
            if( m_godFavor - sw.price() >= 0 )
            {
                Entity::SharedPtr e( new Entity );
                m_godFavor -= sw.price();
                e->position.set(cx, cy);
                e->texture = sw.tex;
                e->sellCost = sw.price() / 2;
                e->etype = sw.createdBuildingType;
                e->tool_widget_index = m_selectedWidgetIndex;
                e->multiplier = sw.efficiencyMultiplier;
                sw.currentItems++;
                switch(sw.createdBuildingType)
                {
                case EntityType::Bank:
                    m_banks.push_back(e);
                    break;
                case EntityType::Virgen:
                    e->bad_texture = m_sheet->getFrame(13);
                    break;
                case EntityType::Cherub:
                    e->bad_texture = m_sheet->getFrame(12);
                    break;
                case EntityType::Cura:
                    e->bad_texture = m_sheet->getFrame(2, 3);
                    break;
                case EntityType::Parisher:
                case EntityType::Door:
                    break;
                }
                if( isPrayBuilding(e) )
                {
                    m_buildings.push_back(e);
                }
                m_buildingEntitiesMap.set(cx, cy, e);
            }
        }
    }

    bool isPrayBuilding(Entity::SharedPtr e)
    {
        switch(e->etype)
        {
        case EntityType::Virgen:
        case EntityType::Cherub:
        case EntityType::Cura:
            return true;
        default:
            return false;
        }
    }

    void trySellBuildingAt( int cx, int cy )
    {
        auto b = m_buildingEntitiesMap.get( cx, cy );
        if( b != nullptr )
        {
            bool do_sell = (b->etype != EntityType::Bank || isBankFree(b)) && ! b->is_bad;
            if( do_sell )
            {
                auto& w = m_widgets[b->tool_widget_index];
                m_buildingEntitiesMap.set(cx, cy, nullptr);
                m_godFavor += b->sellCost;
                w.currentItems--;
                switch(b->etype)
                {
                case EntityType::Bank:
                    auto& v = m_banks;
                    v.erase(std::remove(v.begin(), v.end(), b), v.end());
                    break;
                }
                if( isPrayBuilding(b) )
                {
                    auto& vec = m_buildings;
                    vec.erase(std::remove(vec.begin(), vec.end(), b), vec.end());
                }
            }
        }
    }

    aether::math::Vec2i unproject( int x, int y )
    {
        return aether::math::Vec2i( x / 32, (y - YOffset * 2) / 32);
    }

    void tryIncreaseAscension( int x, int y )
    {
        auto bank = m_buildingEntitiesMap.get(x, y);
        auto parisher = m_frontEntitiesMap.get(x, y);
        if( bank != nullptr && parisher != nullptr && bank->etype == EntityType::Bank && parisher->etype == EntityType::Parisher )
        {
            parisher->ascensionBar += Constants::ClickPrayIncreaseRate;
        }
    }

    bool clicked(bool left_click = true)
    {
        auto& ms = aether::core::_get_mouse_state();
        auto v = unproject(ms.x, ms.y);

        if( v.x() >= 1 && v.x() < Width - 1 &&
            v.y() >= 2 && v.y() < Height - 1 )
        {
            if( left_click )
            {
                tryCreateBuildingAt(v.x(), v.y());
                tryIncreaseAscension(v.x(), v.y());
            }
            else
            {
                trySellBuildingAt(v.x(), v.y());
            }
        }

        if( v.y() == -1 )
        {
            if( v.x() < m_widgets.size() )
            {
                m_selectedWidgetIndex = v.x();
            }
        }

        recomputePrayMultiplier();

        return true;
    }

    void increaseAscension(Entity::SharedPtr e)
    {
        e->ascensionBar += m_prayMultiplier * Constants::BasePrayIncrease;
    }

    bool isBankFree(Entity::SharedPtr e)
    {
        assert(e->etype == EntityType::Bank);
        return m_frontEntitiesMap.get(e->position.x(), e->position.y()) == nullptr;
    }

    void tryAccomodateQueuedParishers()
    {
        if( m_parisherQueue.size() > 0 )
        {
            bool someone_sit = false;
            for( auto b : m_banks )
            {
                if( isBankFree(b) && m_parisherQueue.size() > 0 )
                {
                    auto parisher = m_parisherQueue.front();
                    m_parisherQueue.pop_front();
                    parisher->position = b->position;
                    m_frontEntitiesMap.set(parisher->position.x(), parisher->position.y(), parisher);
                    m_sitParishers.push_back(parisher);
                    someone_sit = true;
                }
            }
            if( someone_sit )
            {
                recomputePrayMultiplier();
            }
        }
    }

    int godFavor()
    {
        return m_godFavor;
    }

    int parishersSaved()
    {
        return m_parishersSaved;
    }

    void recomputePrayMultiplier()
    {
        m_prayMultiplier = 1;
        float num_bads = 0;
        for( auto e : m_buildings )
        {
            if( false == e->is_bad )
            {
                m_prayMultiplier *= e->multiplier;
            }
            else
            {
                num_bads++;
            }
        }
        if( m_prayMultiplier == 1 )
        {
            m_prayMultiplier = 0;
        }
        if( m_sitParishers.size() > 0 )
        {
            m_prayMultiplier /= float(m_sitParishers.size());
        }
    }

    void recomputeApocalypsisLevel()
    {
        if( m_apocalypsisTimer > 1e6 && m_apocalypsisLevel < 100 )
        {
            m_apocalypsisTimer -= 1e6;
            m_apocalypsisLevel += Constants::ApocalypsisLevelIncreasePerSecond;
        }
        m_apocalypsisSpawnModifier = m_apocalypsisLevel / 100.f * Constants::MaxReductionOnSpawnDueToApocalypsisLevel;
    }

    void update(uint64_t delta)
    {
        if( aether::core::is_key_just_pressed(aether::core::KeyCode::R) )
        {
            m_sceneStatus = Status::ManuallyRestarted;
        }

        m_nextParisherUpdate -= float(delta);
        if( m_nextParisherUpdate <= 0 )
        {
            m_nextParisherUpdate += Constants::PrayRefreshRate;
            std::vector<Entity::SharedPtr> to_remove;
            for( auto parisher : m_sitParishers )
            {
                increaseAscension(parisher);
                if( parisher->ascensionBar >= 100 )
                {
                    m_frontEntitiesMap.set(parisher->position.x(), parisher->position.y(), nullptr);
                    m_parishersSaved++;
                    m_apocalypsisLevel -= Constants::ApocalypsisReductionOnSave;
                    if( m_apocalypsisLevel < 0 )
                    {
                        m_apocalypsisLevel = 0;
                    }
                    m_godFavor += Constants::GodFavorGainOnSave;

                    // this parisher is not sit anymore
                    to_remove.push_back(parisher);
                }
            }
            for( auto parisher : to_remove )
            {
                auto& vec = m_sitParishers;
                vec.erase(std::remove(vec.begin(), vec.end(), parisher), vec.end());
            }
            recomputePrayMultiplier();
        }

        m_nextParisherSpawn -= float(delta);
        if( m_nextParisherSpawn <= 0 )
        {
            m_nextParisherSpawn += Constants::SpawnParisherRate - m_apocalypsisSpawnModifier;
            enqueueParisher();
        }

        if( m_buildings.size() > 1 )
        {
            m_nextEvilizeBuilding -= float(delta);
            if( m_nextEvilizeBuilding <= 0 )
            {
                m_nextEvilizeBuilding += Constants::TimeBetweenEvilizes;
                for( int i = 0; i < m_buildings.size(); i++ )
                {
                    Entity::SharedPtr e = m_buildings[i];
                    if( e->is_bad == false )
                    {
                        e->is_bad = true;
                        m_apocalypsisLevel += Constants::ApocalypsisIncreaseOnEvilize;
                        recomputePrayMultiplier();
                        break;
                    }
                }
                int rnd = aether::math::randi(m_buildings.size());
            }
        }

        m_apocalypsisTimer += delta;

        recomputeApocalypsisLevel();

        if( m_apocalypsisLevel >= 100 )
        {
            m_sceneStatus = Scene::Status::LostApocalypsisFullBar;
        }
    }

    ToolWidget& selectedWidget()
    {
        return m_widgets[m_selectedWidgetIndex];
    }

    Status status()
    {
        return m_sceneStatus;
    }

private:

    Status m_sceneStatus = Status::Running;

    float m_nextParisherUpdate = 0;
    float m_nextParisherSpawn = 0;
    float m_nextEvilizeBuilding = Constants::FirstEvilizeBuilding;

    TileMap m_backgroundMap;
    EntityMap m_frontEntitiesMap;
    EntityMap m_buildingEntitiesMap;
    aether::graphics::Spritesheet* m_sheet;
    aether::math::Vec2<size_t> m_cursorPos = {0, 0};
    std::vector<ToolWidget> m_widgets;
    size_t m_selectedWidgetIndex = 0 ;
    std::deque<Entity::SharedPtr> m_parisherQueue;
    std::vector<Entity::SharedPtr> m_banks;
    std::vector<Entity::SharedPtr> m_sitParishers;
    std::vector<Entity::SharedPtr> m_buildings;

    int m_godFavor = Constants::StartingGodFavor;
    int m_parishersSaved = 0;
    float m_prayMultiplier = 1;
    float m_apocalypsisLevel = 5;
    float m_apocalypsisSpawnModifier = 0;
    uint64_t m_apocalypsisTimer = 0;

};
