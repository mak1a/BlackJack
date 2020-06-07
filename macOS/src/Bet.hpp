#pragma once
#include"Common.hpp"

class Bet : public MyApp::Scene {
private:
    RectF m_displayBetMoney;

    RectF m_startButton, m_exitButton;

    Triangle m_betUp, m_betDown;
public:
    Bet(const InitData& init);

    void update() override;

    void draw() const override;
};