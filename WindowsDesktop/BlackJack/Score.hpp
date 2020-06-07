#pragma once
#include"Common.hpp"

class Score : public MyApp::Scene {
private:
    std::array<ScoreData, 5> m_highScores;

public:
    Score(const InitData& init);

    void update() override;

    void draw() const override;
};