#pragma once
#include"Common.hpp"

class Result : public MyApp::Scene {
private:
    std::array<ScoreData, 5> m_highScores;

    const uint32 m_haveMoney;
    const uint32 m_betMoney;
    const uint32 m_winCount;
    const uint32 m_loseCount;

public:
    Result(const InitData& init);

    void update() override;

    void draw() const override;
};