
# include "Title.hpp"

Title::Title(const InitData& init)
: IScene(init)
, m_startButton(Arg::center(Scene::Center().movedBy(0, 50)), 250, 60)
, m_scoreButton(Arg::center(Scene::Center().movedBy(0, 150)), 250, 60)
, m_exitButton(Arg::center(Scene::Center().movedBy(0, 250)), 250, 60)
, m_startTransition(0.4s, 0.2s)
, m_scoreTransition(0.4s, 0.2s)
, m_exitTransition(0.4s, 0.2s) {}

void Title::update() {
	m_startTransition.update(m_startButton.mouseOver());
	m_scoreTransition.update(m_scoreButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_startButton.mouseOver() || m_scoreButton.mouseOver() || m_exitButton.mouseOver()) {
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_startButton.leftClicked()) {
		changeScene(State::Bet);
	}

	if (m_scoreButton.leftClicked()) {
		changeScene(State::Score);
	}

	if (m_exitButton.leftClicked()) {
		System::Exit();
	}
}

void Title::draw() const {
	const double titleHeight = FontAsset(U"Title")(U"Black").region().h;
    
    FontAsset(U"Title")(U"Black").drawAt(Scene::Center().x - 150, titleHeight+20, Palette::Black);
    FontAsset(U"Title")(U"Jack").drawAt(Scene::Center().x + 170, titleHeight+20, Palette::Red);

	m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
	m_scoreButton.draw(ColorF(1.0, m_scoreTransition.value())).drawFrame(2);
	m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

	FontAsset(U"Menu")(U"スタート").drawAt(m_startButton.center(), Palette::Black);
	FontAsset(U"Menu")(U"スコア").drawAt(m_scoreButton.center(), Palette::Black);
	FontAsset(U"Menu")(U"終了").drawAt(m_exitButton.center(), Palette::Black);
}
