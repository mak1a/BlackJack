
# pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
# include <HamFramework.hpp>

// シーンの名前
enum class State {
    Title,	// タイトル
    Bet,	// ベット
    Game,	// ゲーム
    Result,	// リザルト
    Score	// スコア
};

// ゲームデータ
struct GameData {
    uint32 winCount;
    uint32 loseCount;
    uint32 drawCount;

    uint32 betMoney;
    uint32 haveMoney;

    /// <summary>
    /// トランプクラス
    /// </summary>
    // トランプのクラス
    Array<PlayingCard::Card> cards, cemeteryCards;

    GameData() noexcept
        : winCount(0)
        , loseCount(0)
        , drawCount(0)
        , betMoney(1)
        , haveMoney(99)
        , cards(PlayingCard::CreateDeck(0)) {}

    void Init() noexcept {
        cards.clear();
        cemeteryCards.clear();

        winCount = 0;
        loseCount = 0;
        drawCount = 0;
        betMoney = 1;
        haveMoney = 99;
        cards = PlayingCard::CreateDeck(0);
    }
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;

struct ScoreData {
    uint32 score;

    Date date;

    template <class Archive>
    void SIV3D_SERIALIZE(Archive& archive) {
        archive(score, date);
    }
};
