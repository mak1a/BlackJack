# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"
# include "Bet.hpp"
# include "Result.hpp"
# include "Score.hpp"

void Main() {
    // （ウィンドウの閉じるボタンを押したときだけ終了する場合はコメントを外す）
    //System::SetTerminationTriggers(UserAction::CloseButtonClicked);

    Window::Resize(1280, 720);

    // タイトルを設定
    Window::SetTitle(U"Black Jack");

    // 背景色を設定
    Scene::SetBackground(ColorF(0.4, 0.7, 0.5));

    // 使用するフォントアセットを登録
    FontAsset::Register(U"Title", 120, Typeface::Bold, FontStyle::Italic);
    FontAsset::Register(U"Menu", 30, Typeface::Regular);
    FontAsset::Register(U"Version", 14, Typeface::Regular);
    FontAsset::Register(U"Button", 30, Typeface::Light);
    FontAsset::Register(U"Result", 50, Typeface::Bold);
    FontAsset::Register(U"Resultcount", 28, Typeface::Regular);
    FontAsset::Register(U"betMoneyTheme", 50, Typeface::Bold);
    FontAsset::Register(U"ScoreList", 50, Typeface::Heavy);
    FontAsset::Register(U"ScoreListDate", 25, Typeface::Regular, FontStyle::Italic);
    FontAsset::Register(U"PlayerScore", 50, Typeface::Bold);
    FontAsset::Register(U"GameResult", 120, Typeface::Regular);
    FontAsset::Register(U"KeyEnterDown", 35, Typeface::Regular);
    FontAsset::Register(U"betMoney", 70, Typeface::Regular);
    FontAsset::Register(U"betMoneyExplain", 35, Typeface::Regular);

    TextureAsset::Register(U"Deck", Resource(U"texture/cardgame_deck_hiku.png"));
    TextureAsset::Register(U"CardMan", Resource(U"texture/cardgame_card_dasu.png"));

    // シーンと遷移時の色を設定
    MyApp manager;
    manager
        .setFadeColor(Palette::Black)
        .add<Title>(State::Title)
        .add<Bet>(State::Bet)
        .add<Game>(State::Game)
        .add<Result>(State::Result)
        .add<Score>(State::Score)
        .setFadeColor(ColorF(1.0));

    // （ゲームシーンから開始する場合はコメントを外す）
    //manager.init(State::Game);

    while (System::Update()) {
        if (!manager.update()) {
            break;
        }
    }
}

