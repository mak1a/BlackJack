#include "Result.hpp"

Result::Result(const InitData& init)
    : IScene(init)
    , m_highScores({
        ScoreData{0, Date(2020,1,1)},
        ScoreData{0, Date(2020,1,1)},
        ScoreData{0, Date(2020,1,1)},
        ScoreData{0, Date(2020,1,1)},
        ScoreData{0, Date(2020,1,1)}
        })
    , m_betMoney(getData().betMoney)
    , m_haveMoney(getData().haveMoney)
    , m_winCount(getData().winCount)
    , m_loseCount(getData().loseCount) {
    if (FileSystem::Exists(U"saves/score.dat")) {
        Deserializer<BinaryReader>{U"saves/score.dat"}(m_highScores);
    }
    else {
        Serializer<BinaryWriter>{U"saves/score.dat"}(m_highScores);
    }

    if (m_highScores.back().score <= (m_haveMoney + m_betMoney)) {
        m_highScores.back() = { (m_haveMoney + m_betMoney), Date::Today() };

        std::sort(m_highScores.begin(), m_highScores.end(), [](const ScoreData& a, const ScoreData& b) {
            return a.score > b.score ? true : a.score == b.score ? a.date > b.date : false;
            });

        Serializer<BinaryWriter>{U"saves/score.dat"}(m_highScores);
    }
}

void Result::update() {
    // �^�C�g����ʂɖ߂鏈��
    if (MouseL.down()) {
        getData().Init();
        changeScene(State::Title);
    }
}

void Result::draw() const {
    const double resultHeight = FontAsset(U"Result")(U"�������F", (m_haveMoney + m_betMoney)).region().h;

    FontAsset(U"Result")(U"�������F", (m_haveMoney + m_betMoney)).drawAt(Scene::CenterF().x, Scene::Height() * 0.4 - resultHeight / 2, Palette::Black);

    FontAsset(U"Resultcount")(U"�i�������v�������F", m_winCount, U"�j").drawAt(Scene::CenterF().x, Scene::Height() * 0.4 - resultHeight / 2 + 100, Palette::Black);

    FontAsset(U"Resultcount")(U"�i�������v�s�k���F", m_loseCount, U"�j").drawAt(Scene::CenterF().x, Scene::Height() * 0.4 - resultHeight / 2 + 160, Palette::Black);
}
