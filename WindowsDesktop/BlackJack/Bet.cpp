#include "Bet.hpp"

Bet::Bet(const InitData& init)
    : IScene(init)
    , m_displayBetMoney(Arg::center(Scene::Center().moveBy(0, 50)), 200, 100)
    , m_startButton(Arg::center(770, 560), 150, 60)
    , m_exitButton(Arg::center(510, 560), 150, 60)
    , m_betUp(m_displayBetMoney.tr().movedBy(30, 0), m_displayBetMoney.tr().movedBy(20, 15), m_displayBetMoney.tr().movedBy(40, 15))
    , m_betDown(m_displayBetMoney.br().movedBy(30, 0), m_displayBetMoney.br().movedBy(20, -15), m_displayBetMoney.br().movedBy(40, -15)) {}

void Bet::update() {
    // ゲームオーバーの場合
    if (getData().haveMoney == 0 && getData().betMoney == 0) {
        if (MouseL.down()) {
            changeScene(State::Result);
        }
        return;
    }

    if (m_betUp.mouseOver() || m_betDown.mouseOver() || m_startButton.mouseOver() || m_exitButton.mouseOver()) {
        Cursor::RequestStyle(CursorStyle::Hand);
    }

    // 賭け金の設定
    if (m_betUp.leftClicked()) {
        if (getData().haveMoney > 0) {
            --getData().haveMoney;
            ++getData().betMoney;
            return;
        }
    }
    else if (m_betDown.leftClicked()) {
        if (getData().betMoney > 1) {
            ++getData().haveMoney;
            --getData().betMoney;
            return;
        }
    }

    if (m_startButton.leftClicked()) {
        changeScene(State::Game, 0s);
        return;
    }
    if (m_exitButton.leftClicked()) {
        changeScene(State::Result);
        return;
    }

}

void Bet::draw() const {
    if (getData().haveMoney == 0 && getData().betMoney == 0) {
        FontAsset(U"betMoney")(U"ゲームオーバー...").drawAt(Scene::Center().movedBy(0, -100), Palette::Black);
        FontAsset(U"betMoneyExplain")(U"クリックして下さい").drawAt(Scene::Center().movedBy(0, 100), Palette::Black);
        return;
    }

    FontAsset(U"KeyEnterDown")(U"終了する場合は終了ボタンを押してください").drawAt(Scene::Center().movedBy(0, 300), Palette::Black);

    FontAsset(U"betMoneyTheme")(U"賭け金を設定してください").drawAt(Scene::Center().x, 120, Palette::Black);
    FontAsset(U"KeyEnterDown")(U"決定はスタートボタンを押してください").drawAt(Scene::Center().movedBy(0, -170), Palette::Black);
    FontAsset(U"betMoneyExplain")(U"残金：", getData().haveMoney).drawAt(Scene::Center().x, 300, Palette::Black);

    m_displayBetMoney.draw(Palette::White);

    m_startButton.draw(Palette::White);
    m_startButton.drawFrame(2.0, Palette::Black);
    FontAsset(U"Menu")(U"スタート").drawAt(m_startButton.center(), Palette::Black);

    m_exitButton.draw(Palette::White);
    m_exitButton.drawFrame(2.0, Palette::Black);
    FontAsset(U"Menu")(U"やめる").drawAt(m_exitButton.center(), Palette::Black);

    m_betUp.draw(Palette::Lightpink);
    m_betUp.drawFrame(0.8, Palette::Black);
    m_betDown.draw(Palette::Lightpink);
    m_betDown.drawFrame(0.8, Palette::Black);

    FontAsset(U"betMoney")(getData().betMoney).drawAt(Scene::Center().movedBy(0, 50), Palette::Black);
}
