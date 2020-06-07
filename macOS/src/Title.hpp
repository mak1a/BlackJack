
# pragma once
# include "Common.hpp"

// タイトルシーン
class Title : public MyApp::Scene {
private:
    Rect m_startButton;
    Transition m_startTransition;

    Rect m_scoreButton;
    Transition m_scoreTransition;

    Rect m_exitButton;
    Transition m_exitTransition;

public:
    Title(const InitData& init);

    void update() override;

    void draw() const override;
};
