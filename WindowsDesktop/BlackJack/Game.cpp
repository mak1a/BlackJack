
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
    , m_drawCount(getData().drawCount)
    , m_winner(Winner::Draw) {
    // �J�[�h���V���b�t������
    getData().cards.shuffle();
}

void Game::DistributeInit() {
    // �R�D��4�������������ꍇ
    if (getData().cards.size() < 4) {
        // ��n�̃J�[�h���V���b�t������
        getData().cemeteryCards.shuffle();
        // ��n�̃J�[�h���Ȃ��Ȃ�܂�
        while (!getData().cemeteryCards.isEmpty()) {
            // ��n����R�D�ɃJ�[�h��߂�
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // �z��̂��G���̎����A1�����Ɏ����Ă�ꍇ
    if (m_enemyCards.size() == 1 && m_distributeEnemyTimer.isRunning() && m_distributeEnemyTimer >= 0.5s) {
        // �z��g�����v�𗠖ʂɂ���
        getData().cards[0].flip();
    }

    // �z��̂��G���̎�
    if (m_distributeEnemyTimer.isRunning() && m_distributeEnemyTimer >= 0.5s) {
        // �G�ɃJ�[�h��z��A�������I������
        m_enemyCards.emplace_back(getData().cards[0]);
        getData().cards.pop_front();
        m_distributeEnemyTimer.reset();
        m_distributeTimer.restart();

        CalucurateEnemyScore();
        return;
    }

    // �z��̂��v���C���[���̏ꍇ
    if (m_distributeTimer.isRunning() && m_distributeTimer >= 0.5s) {
        // �v���C���[�ɃJ�[�h��z��
        m_playerCards.emplace_back(getData().cards[0]);
        getData().cards.pop_front();

        // �v���C���[�̎����Ă���J�[�h��1���̏ꍇ
        if (m_playerCards.size() == 1) {
            // �������I������
            m_distributeEnemyTimer.restart();
            m_distributeTimer.reset();

            CalucuratePlayerScore();
            return;
        }

        // �z�鏈�����I������
        m_distributeEnemyTimer.reset();
        m_distributeTimer.reset();
        m_turn = Turn::Player;

        CalucuratePlayerScore();
    }
}

void Game::DistributeChild() {
    // �R�D�̖�����0���������ꍇ
    if (getData().cards.isEmpty()) {
        // ��n�̃J�[�h���V���b�t������
        getData().cemeteryCards.shuffle();

        // ��n�̃J�[�h���Ȃ��Ȃ�܂�
        while (!getData().cemeteryCards.isEmpty()) {
            // ��n����R�D�ɃJ�[�h��߂�
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // �v���C���[���g�����v������9���ȏ㎝���Ă��ꍇ�A
    // �v���C���[�͔s�k���Ă��邱�ƂɂȂ�̂ŏ������I������
    if (m_playerCards.size() > 9) {
        CalucuratePlayerScore();
        return;
    }

    // �v���C���[�Ƀg�����v��z��
    m_playerCards.emplace_back(getData().cards[0]);
    getData().cards.pop_front();

    CalucuratePlayerScore();
}

void Game::DistributeParent() {
    // �R�D�̖�����0���������ꍇ
    if (getData().cards.isEmpty()) {
        // ��n�̃J�[�h���V���b�t������
        getData().cemeteryCards.shuffle();

        // ��n�̃J�[�h���Ȃ��Ȃ�܂�
        while (!getData().cemeteryCards.isEmpty()) {
            // ��n����R�D�ɃJ�[�h��߂�
            getData().cards.emplace_back(getData().cemeteryCards[0]);
            getData().cemeteryCards.pop_front();
        }
    }

    // �G�Ƀg�����v��z��
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

    // 21�_�𒴂����ꍇ�A���U���g��
    if (m_playerScore > 21) {
        /// <summary>
        /// ���̎��_�ŏ��҂����肷��
        /// </summary>
        DeterminWinner();
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

    // 17�_�𒴂����ꍇ�A���U���g��
    if (m_enemyScore >= 17) {
        /// <summary>
        /// ���̎��_�ŏ��҂����肷��
        /// </summary>
        DeterminWinner();
        m_distributeEnemyTimer.reset();
        m_turn = Turn::Result;
    }
}

void Game::CollectCard() {
    // �q�����̏���
    switch (m_winner) {
    case Winner::Player:
        ++getData().winCount;
        getData().haveMoney += getData().betMoney;
        break;
    case Winner::Enemy:
        ++getData().loseCount;

        if (getData().haveMoney == 0) {
            getData().betMoney = 0;
        }
        else {
            --getData().haveMoney;
            getData().betMoney = 1;
        }
        break;
    case Winner::Draw:
        ++getData().drawCount;
        break;
    default:
        break;
    }

    // �v���C���[�̃J�[�h���Ȃ��Ȃ�܂�
    while (!m_playerCards.isEmpty()) {
        // �v���C���[�̃J�[�h���n�ɑ���
        getData().cemeteryCards.emplace_back(m_playerCards[0]);
        m_playerCards.pop_front();
    }

    // �G�̃J�[�h���Ȃ��Ȃ�܂�
    while (!m_enemyCards.isEmpty()) {
        // �������J�[�h�����������Ă��ꍇ�A�\�ɂ���
        if (!m_enemyCards[0].isFaceSide) {
            m_enemyCards[0].flip();
        }

        // �G�̃J�[�h���n�ɑ���
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

        // �X�R�A��21�𒴂��Ă��ꍇ�A���ʉ�ʂɂ���
        if (m_playerScore > 21) {
            m_turn = Turn::Result;
            return;
        }

        // �q�b�g
        if (m_hitButton.leftClicked()) {
            DistributeChild();
            return;
        }
        // �X�^���h
        // ����̃^�[���ɂ���
        if (m_standButton.leftClicked()) {
            m_turn = Turn::Enemy;
            m_distributeEnemyTimer.restart();

            // 2���ڂ̃J�[�h��\�����ɂ��Ă�����
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
    FontAsset(U"ScoreListDate")(U"�c��̎R�D�̐��F", getData().cards.size(), U"��").drawAt(1050, 300);
    FontAsset(U"ScoreListDate")(U"�q�����F", m_betMoney, U"�y�\").drawAt(1050, 350);
    FontAsset(U"ScoreListDate")(U"�c��c���F", m_haveMoney, U"�y�\").drawAt(1050, 400);

    FontAsset(U"ScoreListDate")(U"�������F", m_winCount + m_loseCount + m_drawCount, U"��").drawAt(150, 100);
    FontAsset(U"ScoreListDate")(U"�i�����������F", m_winCount, U"��j").drawAt(150, 150);
    FontAsset(U"ScoreListDate")(U"�i�����s�k���F", m_loseCount, U"��j").drawAt(150, 200);

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

    switch (m_winner) {
    case Winner::Player:
        FontAsset(U"GameResult")(U"Win").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Red);
        break;
    case Winner::Enemy:
        FontAsset(U"GameResult")(U"Lose").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Darkblue);
        break;
    case Winner::Draw:
        FontAsset(U"GameResult")(U"Draw").drawAt(Scene::CenterF().moveBy(0, -55), Palette::Black);
        break;
    default:
        break;
    }

    FontAsset(U"KeyEnterDown")(U"�N���b�N���Ă�������").drawAt(Scene::CenterF().moveBy(0, 320), Palette::Black);
}
