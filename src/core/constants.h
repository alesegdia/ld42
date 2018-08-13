#pragma once

class Constants
{
public:
    static constexpr int    BankCost = 10;
    static constexpr float  BankCostMultiplier = 1.07f;

    static constexpr int    VirgenRocioCost = 10;
    static constexpr float  VirgenRocioCostMultiplier = 1.1f;
    static constexpr float  VirgenRocioPrayEfficiencyMultiplier = 1.2f;

    static constexpr int    PriestCost = 220;
    static constexpr float  PriestCostMultiplier = 1.1f;
    static constexpr float  PriestPrayEfficiencyMultiplier = 1.7f;

    static constexpr int    CherubCost = 600;
    static constexpr float  CherubCostMultiplier = 1.1f;
    static constexpr float  CherubPrayEfficiencyMultiplier = 2.1f;

    static constexpr float  BasePrayIncrease = 1.f;

    static constexpr int    GodFavorGainOnSave = 100;
    static constexpr int    StartingGodFavor = 200;
    static constexpr int    MaxGainedFavorOnApocalypsisLevel = 30;

    static constexpr int    PrayRefreshRate = 250000;

    static constexpr int    SpawnParisherRate = 4e6;
    static constexpr float  MaxReductionOnSpawnDueToApocalypsisLevel = 3.5e6;

    static constexpr float  FirstEvilizeBuilding = 20e6;
    static constexpr float  TimeBetweenEvilizes = 20e6;

    static constexpr float  ClickPrayIncreaseRate = 1.f;

    static constexpr float  ApocalypsisReductionOnSave = 2.5f;
    static constexpr float  ApocalypsisIncreaseOnEvilize = 20.f;

    static constexpr float  ApocalypsisLevelIncreasePerSecond = 1.f;

};
