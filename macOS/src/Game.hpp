
# pragma once
# include "Common.hpp"

// ゲームシーン
class Game : public MyApp::Scene {
private:
    /// <summary>
    /// トランプの描画クラス
    /// </summary>
    PlayingCard::Pack m_pack;

    /// <summary>
    /// タイマー
    /// </summary>
    Stopwatch m_distributeTimer, m_distributeEnemyTimer;

    /// <summary>
    /// トランプクラス
    /// </summary>
    // トランプのクラス
    Array<PlayingCard::Card> m_playerCards, m_enemyCards;

    /// <summary>
    /// ヒット、スタンドを選ぶボタン
    /// </summary>
    RectF m_hitButton, m_standButton;

    /// <summary>
    /// どちらのターンか
    /// </summary>
    enum class Turn {
        Distribute,	// 両者にカードを2枚ずつ配る
        Player,	// Playerのターン
        Enemy,	// Enemyのターン
        Result	// 結果表示
    } m_turn;

    enum class SelectButton {
        Hit,
        Stand,
        None
    } m_select;

    /// <summary>
    /// Aを持っている枚数
    /// </summary>
    uint32 m_countAce, m_countAceEnemy;

    /// <summary>
    /// 点数
    /// </summary>
    uint32 m_playerScore, m_enemyScore;

    /// <summary>
    /// 賭け金
    /// シーン移動時に数字が変わらないようにする
    /// </summary>
    const uint32 m_betMoney;

    /// <summary>
    /// 所持金
    /// シーン移動時に数字が変わらないようにする
    /// </summary>
    const uint32 m_haveMoney;

    /// <summary>
    /// 勝利数
    /// シーン移動時に数字が変わらないようにする
    /// </summary>
    const uint32 m_winCount;

    /// <summary>
    /// 敗北数
    /// シーン移動時に数字が変わらないようにする
    /// </summary>
    const uint32 m_loseCount;

    /// <summary>
    /// 引き分け数
    /// シーン移動時に数字が変わらないようにする
    /// </summary>
    const uint32 m_drawCount;

    enum class Winner {
        Player,
        Enemy,
        Draw
    } m_winner;

    /// <summary>
    /// 勝者を決定する
    /// </summary>
    void DeterminWinner() noexcept {
        /// <summary>
        /// プレイヤーのみがブラックジャックならプレイヤーの勝利
        /// </summary>
        if (m_playerCards.size() == 2) {
            if (((m_playerCards[0].isAce() && m_playerCards[1].rank >= 10) || (m_playerCards[1].isAce() && m_playerCards[0].rank >= 10))
                && !((m_enemyCards[0].isAce() && m_enemyCards[1].rank >= 10) || (m_enemyCards[1].isAce() && m_enemyCards[0].rank >= 10))) {
                m_winner = Winner::Player;
                return;
            }
        }

        /// <summary>
        /// 敵のみがブラックジャックなら敵の勝利
        /// </summary>
        if (m_enemyCards.size() == 2) {
            if (((m_enemyCards[0].isAce() && m_enemyCards[1].rank >= 10) || (m_enemyCards[1].isAce() && m_enemyCards[0].rank >= 10))
                && !((m_playerCards[0].isAce() && m_playerCards[1].rank >= 10) || (m_playerCards[1].isAce() && m_playerCards[0].rank >= 10))) {
                m_winner = Winner::Enemy;
                return;
            }
        }

        if (m_playerScore <= 21 && m_playerScore == m_enemyScore) {
            m_winner = Winner::Draw;
            return;
        }

        if (m_playerScore <= 21 && (m_playerScore > m_enemyScore || m_enemyScore > 21)) {
            m_winner = Winner::Player;
            return;
        }

        m_winner = Winner::Enemy;
    }
    
    /// <summary>
    /// プレイヤーにカードを配る処理
    /// </summary>
    void DistributeChild();

    /// <summary>
    /// 敵にカードを配る処理
    /// </summary>
    void DistributeParent();

    /// <summary>
    /// ゲーム開始直後に両者に配る処理
    /// 点数なども初期化する
    /// </summary>
    void DistributeInit();

    /// <summary>
    /// プレイヤーのトランプの合計値を計算する処理
    /// </summary>
    void CalucuratePlayerScore();

    /// <summary>
    /// エネミーのトランプの合計値を計算する処理
    /// </summary>
    void CalucurateEnemyScore();

    /// <summary>
    /// 場に出てるトランプを回収する
    /// 同時に賭け金の回収などもする
    /// </summary>
    void CollectCard();

public:
    Game(const InitData& init);

    void update() override;

    void draw() const override;
};
