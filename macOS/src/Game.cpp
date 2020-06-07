
# include "Game.hpp"

Game::Game(const InitData& init)
: IScene(init)
, m_pack(75, Palette::Red)
, m_countAce(0)
, m_countAceEnemy(0)
, m_hitButton(Vec2(Scene::CenterF().x - 150.0, 600.0), 100.0, 40.0)
, m_standButton(Vec2(Scene::CenterF().x + 70.0, 600.0), 100.0, 40.0)
, m_distributeTimer(false)
, m_distributeEnemyTimer(true)
, m_turn(Turn::Distribute)
, m_select(SelectButton::None)
, m_playerScore(0)
, m_enemyScore(0)
, m_betMoney(getData().betMoney)
, m_haveMoney(getData().haveMoney)
, m_winCount(getData().winCount)
, m_loseCount(getData().loseCount)
, m_drawCount(getData().drawCount) {
    // カードをシャッフルする
    getData().cards.shuffle();
}

void Game::DistributeInit() {
    // 山札が4枚未満だった場合
    if (getData().cards.size() < 4) {
        // 墓地のカードをシャッフルする
        getData().cemeteryCards.shuffle();
        // 墓地のカードがなくなるまで
        while (!getData().cemeteryCards.isEmpty()) {
            // 墓地から山札にカードを戻す
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // 配るのが敵側の時かつ、1枚既に持ってる場合
    if (m_enemyCards.size() == 1 && m_distributeEnemyTimer.isRunning() && m_distributeEnemyTimer >= 0.5s) {
        // 配るトランプを裏面にする
        getData().cards[0].flip();
    }

    // 配るのが敵側の時
    if (m_distributeEnemyTimer.isRunning() && m_distributeEnemyTimer >= 0.5s) {
        // 敵にカードを配り、処理を終了する
        m_enemyCards.emplace_back(getData().cards[0]);
        getData().cards.pop_front();
        m_distributeEnemyTimer.reset();
        m_distributeTimer.restart();

        CalucurateEnemyScore();
        return;
    }

    // 配るのがプレイヤー側の場合
    if (m_distributeTimer.isRunning() && m_distributeTimer >= 0.5s) {
        // プレイヤーにカードを配る
        m_playerCards.emplace_back(getData().cards[0]);
        getData().cards.pop_front();

        // プレイヤーの持っているカードが1枚の場合
        if (m_playerCards.size() == 1) {
            // 処理を終了する
            m_distributeEnemyTimer.restart();
            m_distributeTimer.reset();

            CalucuratePlayerScore();
            return;
        }

        // 配る処理を終了する
        m_distributeEnemyTimer.reset();
        m_distributeTimer.reset();
        m_turn = Turn::Player;

        CalucuratePlayerScore();
    }
}

void Game::DistributeChild() {
    // 山札の枚数が0枚だった場合
    if (getData().cards.isEmpty()) {
        // 墓地のカードをシャッフルする
        getData().cemeteryCards.shuffle();

        // 墓地のカードがなくなるまで
        while (!getData().cemeteryCards.isEmpty()) {
            // 墓地から山札にカードを戻す
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // プレイヤーがトランプを既に9枚以上持ってた場合、
    // プレイヤーは敗北していることになるので処理を終了する
    if (m_playerCards.size() > 9) {
        CalucuratePlayerScore();
        return;
    }

    // プレイヤーにトランプを配る
    m_playerCards.emplace_back(getData().cards[0]);
    getData().cards.pop_front();

    CalucuratePlayerScore();
}

void Game::DistributeParent() {
    // 山札の枚数が0枚だった場合
    if (getData().cards.isEmpty()) {
        // 墓地のカードをシャッフルする
        getData().cemeteryCards.shuffle();

        // 墓地のカードがなくなるまで
        while (!getData().cemeteryCards.isEmpty()) {
            // 墓地から山札にカードを戻す
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // 敵にトランプを配る
    m_enemyCards.emplace_back(getData().cards[0]);
    getData().cards.pop_front();

    CalucurateEnemyScore();
}

void Game::CalucuratePlayerScore() {
    m_countAce = 0;
    m_playerScore = 0;

    for (const auto& playerCard : m_playerCards) {
        if (playerCard.isAce()) {
            ++m_countAce;
            m_playerScore += 11;
        }
        else if (playerCard.rank <= 10) {
            m_playerScore += playerCard.rank;
        }
        else {
            m_playerScore += 10;
        }
    }

    if (m_countAce > 0 && m_playerScore > 21) {
        for (uint32 count{}; m_countAce != count; ++count) {
            m_playerScore -= 10;
            if (m_playerScore <= 21) {
                break;
            }
        }
    }

    // 21点を超えた場合、リザルトへ
    if (m_playerScore > 21) {
        m_turn = Turn::Result;
    }
}

void Game::CalucurateEnemyScore() {
    m_countAceEnemy = 0;
    m_enemyScore = 0;

    for (const auto& enemyCard : m_enemyCards) {
        if (!enemyCard.isFaceSide) {
            break;
        }

        if (enemyCard.isAce()) {
            ++m_countAceEnemy;
            m_enemyScore += 11;
        }
        else if (enemyCard.rank <= 10) {
            m_enemyScore += enemyCard.rank;
        }
        else {
            m_enemyScore += 10;
        }
    }

    if (m_countAceEnemy > 0 && m_enemyScore > 21) {
        for (uint32 count{}; m_countAceEnemy != count; ++count) {
            m_enemyScore -= 10;

            if (m_enemyScore <= 21) {
                break;
            }
        }
    }

    // 17点を超えた場合、リザルトへ
    if (m_enemyScore > 17) {
        m_distributeEnemyTimer.reset();
        m_turn = Turn::Result;
    }
}

void Game::CollectCard() {
    // 賭け金の処理
    if (!IsPlayerWin() && !IsDrawGame()) {
        ++getData().loseCount;

        if (getData().haveMoney == 0) {
            getData().betMoney = 0;
        }
        else {
            --getData().haveMoney;
            getData().betMoney = 1;
        }
    }
    else if (IsPlayerWin()) {
        ++getData().winCount;
        getData().haveMoney += getData().betMoney;
    }
    else {
        ++getData().drawCount;
    }

    // プレイヤーのカードがなくなるまで
    while (!m_playerCards.isEmpty()) {
        // プレイヤーのカードを墓地に送る
        getData().cemeteryCards.emplace_back(m_playerCards[0]);
        m_playerCards.pop_front();
    }

    // 敵のカードがなくなるまで
    while (!m_enemyCards.isEmpty()) {
        // もしもカードが裏を向いてた場合、表にする
        if (!m_enemyCards[0].isFaceSide) {
            m_enemyCards[0].flip();
        }

        // 敵のカードを墓地に送る
        getData().cemeteryCards.emplace_back(m_enemyCards[0]);
        m_enemyCards.pop_front();
    }
}

void Game::update() {
    switch (m_turn) {
    case Turn::Distribute:
        DistributeInit();
        return;
    case Turn::Player:
        if (m_hitButton.mouseOver()) {
            Cursor::RequestStyle(CursorStyle::Hand);
            m_select = SelectButton::Hit;
        }
        else if (m_standButton.mouseOver()) {
            Cursor::RequestStyle(CursorStyle::Hand);
            m_select = SelectButton::Stand;
        }
        else {
            m_select = SelectButton::None;
        }

        // スコアが21を超えてた場合、結果画面にする
        if (m_playerScore > 21) {
            m_turn = Turn::Result;
            return;
        }

        // ヒット
        if (m_hitButton.leftClicked()) {
            DistributeChild();
            return;
        }
        // スタンド
        // 相手のターンにする
        if (m_standButton.leftClicked()) {
            m_turn = Turn::Enemy;
            m_distributeEnemyTimer.restart();

            // 2枚目のカードを表向きにしてあげる
            m_enemyCards[1].flip();
            CalucurateEnemyScore();
        }
        return;
    case Turn::Enemy:
        if (m_distributeEnemyTimer >= 0.5s) {
            m_distributeEnemyTimer.restart();
            DistributeParent();
        }
        return;
    case Turn::Result:
        if (MouseL.down()) {
            CollectCard();
            changeScene(State::Bet, 0s);
        }
        return;
    }
}

void Game::draw() const {
    FontAsset(U"ScoreListDate")(U"残りの山札の数：", getData().cards.size(), U"枚").drawAt(1050, 300);
    FontAsset(U"ScoreListDate")(U"賭け金：", m_betMoney, U"ペソ").drawAt(1050, 350);
    FontAsset(U"ScoreListDate")(U"残り残金：", m_haveMoney, U"ペソ").drawAt(1050, 400);
    
    FontAsset(U"ScoreListDate")(U"試合数：", m_winCount + m_loseCount + m_drawCount, U"回").drawAt(150, 100);
    FontAsset(U"ScoreListDate")(U"（うち勝利数：", m_winCount, U"回）").drawAt(150, 150);
    FontAsset(U"ScoreListDate")(U"（うち敗北数：", m_loseCount, U"回）").drawAt(150, 200);

    TextureAsset(U"Deck").scaled(0.3).drawAt(1050, 200);
    TextureAsset(U"CardMan").scaled(0.7).drawAt(170, 450);
    
    for (const auto [i, card] : IndexedRef(m_playerCards)) {
        m_pack(card).drawAt(Scene::CenterF().x - 80 + i * 80, 500);
    }
    
    for (const auto [i, card] : IndexedRef(m_enemyCards)) {
        m_pack(card).drawAt(Scene::CenterF().x - 80 + i * 80, 100);
    }
    
    if (m_select == SelectButton::Hit) {
        m_hitButton.draw(Palette::White);
        m_hitButton.drawFrame(3, Palette::Yellow);

        m_standButton.draw(Palette::Ghostwhite);
        m_standButton.drawFrame(1.0, Palette::Black);
    }
    else if (m_select == SelectButton::Stand) {
        m_hitButton.draw(Palette::Ghostwhite);
        m_hitButton.drawFrame(1.0, Palette::Black);

        m_standButton.draw(Palette::White);
        m_standButton.drawFrame(3, Palette::Yellow);
    }
    else {
        m_hitButton.draw(Palette::Ghostwhite);
        m_hitButton.drawFrame(1.0, Palette::Black);

        m_standButton.draw(Palette::Ghostwhite);
        m_standButton.drawFrame(1.0, Palette::Black);
    }
    FontAsset(U"Button")(U"Hit").drawAt(m_hitButton.center(), Palette::Black);
    
    FontAsset(U"Button")(U"Stand").drawAt(m_standButton.center(), Palette::Black);
    
    FontAsset(U"PlayerScore")(m_enemyScore).drawAt(Scene::CenterF().x, 200);
    FontAsset(U"PlayerScore")(m_playerScore).drawAt(Scene::CenterF().x, 400);
    
    if (m_turn != Turn::Result) {
        return;
    }
    
    if (IsPlayerWin()) {
        FontAsset(U"GameResult")(U"Win").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Red);
    }
    else if (IsDrawGame()) {
        FontAsset(U"GameResult")(U"Draw").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Black);
    }
    else {
        FontAsset(U"GameResult")(U"Lose").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Darkblue);
    }
    FontAsset(U"KeyEnterDown")(U"クリックしてください").drawAt(Scene::CenterF().moveBy(0, 320), Palette::Black);
}
