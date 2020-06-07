#include "Score.hpp"

Score::Score(const InitData& init)
: IScene(init)
, m_highScores({
    ScoreData{0, Date(2020,1,1)},
    ScoreData{0, Date(2020,1,1)},
    ScoreData{0, Date(2020,1,1)},
    ScoreData{0, Date(2020,1,1)},
    ScoreData{0, Date(2020,1,1)}
}) {
    if (FileSystem::Exists(U"saves/score.dat")) {
        Deserializer<BinaryReader>{U"saves/score.dat"}(m_highScores);
    }
    else {
        Serializer<BinaryWriter>{U"saves/score.dat"}(m_highScores);
    }
}

void Score::update() {
    // タイトル画面に戻る処理
    if (MouseL.down()) {
        changeScene(State::Title);
    }
}

void Score::draw() const {
    const int32 h = FontAsset(U"ScoreList").height();
    
    for (const auto [i, highScore] : IndexedRef(m_highScores))
    {
        const Rect rect = Rect(520, 100).setCenter(Scene::CenterF().x + 100, static_cast<int>(120 + i * 120));
        
        rect.draw(ColorF(1.0, 0.2));
        
        FontAsset(U"ScoreList")(i + 1, U"位").draw(252, rect.pos.y + (rect.h - h) / 2 + 2, Palette::Gray);
        FontAsset(U"ScoreList")(i + 1, U"位").draw(250, rect.pos.y + (rect.h - h) / 2, Palette::Black);
        
        FontAsset(U"ScoreList")(highScore.score)
        .draw(rect.pos + Point(42, (rect.h - h) / 2 + 2), Palette::Gray);
        
        FontAsset(U"ScoreList")(highScore.score)
        .draw(rect.pos + Point(40, (rect.h - h) / 2), Palette::Black);
        
        const Size dateSize = FontAsset(U"ScoreListDate")(highScore.date).region().size;
        
        FontAsset(U"ScoreListDate")(highScore.date)
        .draw(rect.br().x - dateSize.x - 40, rect.center().y - dateSize.y / 2, Palette::Black);
    }
}
