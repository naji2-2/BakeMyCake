#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <random>

using namespace sf;
using namespace std;

enum class GameState {
    StartScreen,
    OrderScreen
};

wstring flavor[] = { L"생크림", L"초코" };
wstring topping[] = { L"딸기", L"체리" };
string topping_num[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
int gender[] = { 1, 2 };
int f, t, t_num;

// 시작화면 클래스
class StartScreen {
public:
    StartScreen() {
        if (!backgroundTexture.loadFromFile("Images/Start_Seen_N.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );
        if (!buttonTexture.loadFromFile("Images/Start_Button.png")) {
            cout << "Failed to load button image" << endl;
        }
        buttonSprite.setTexture(buttonTexture);
        buttonSprite.setPosition(1026.f, 1325.f);
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(buttonSprite);
    }

    bool isStartButtonPressed(Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    Sprite buttonSprite;
    Texture buttonTexture;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    const Vector2u windowSize = { 2880, 1800 };
};

// 주문 화면 클래스
class OrderScreen {
public:
    OrderScreen() {
        if (!backgroundTexture.loadFromFile("Images/Order_Seen.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        random_device rd;   // 난수 생성 갹체
        mt19937 gen(rd());
        uniform_int_distribution<int> two(0, 1);
        uniform_int_distribution<int> top(0, 7);

        gender = two(gen);
        f = two(gen);
        t = two(gen);
        t_num = top(gen);

        if (gender == 0) {
            if (!characterTexture.loadFromFile("Images/male.png")) {
                cout << "Failed to load male character image" << endl;
            }
        }
        else {
            if (!characterTexture.loadFromFile("Images/female.png")) {
                cout << "Failed to load female character image" << endl;
            }
        }
        characterSprite.setTexture(characterTexture);
        characterSprite.setPosition(250.f, 756.f);

        if (!balloonTexture.loadFromFile("Images/Text_Balloon.png")) {
            cout << "Failed to load Text Balloon image" << endl;
        }
        balloonSprite.setTexture(balloonTexture);
        balloonSprite.setPosition(149.f, 136.f);
        balloonSprite.setColor(Color(255, 255, 255, balloonAlpha));

        // 폰트 로드
        if (!font.loadFromFile("Fonts/JejuGothic.ttf")) {
            cout << "Failed to load font" << endl;
        }

        // 토핑 (딸기, 체리)
        toppingText.setFont(font);
        toppingText.setString(topping[t]);
        toppingText.setCharacterSize(100);          // 글자 크기 설정
        toppingText.setFillColor(Color::Red);      // 글자 색상
        toppingText.setPosition(260.f, 291.f);      // 텍스트 위치 설정

        balloonText1.setFont(font);
        balloonText1.setString(L"가 들어간 케이크가 먹고 싶어요!");
        balloonText1.setCharacterSize(96);
        balloonText1.setFillColor(Color::Black);
        balloonText1.setPosition(445.f, 292.f);

        // 맛 (생크림, 초콜릿)
        flavorText.setFont(font);
        flavorText.setString(flavor[f]);
        flavorText.setCharacterSize(100);
        flavorText.setFillColor(Color::Blue);
        if (f == 0)
            flavorText.setPosition(300.f, 470.f);
        else
            flavorText.setPosition(400.f, 470.f);

        ballonnText2.setFont(font);
        ballonnText2.setString(L" 케이크에 ");
        ballonnText2.setCharacterSize(96);
        ballonnText2.setFillColor(Color::Black);
        ballonnText2.setPosition(570.f, 474.f);

        // 토핑 갯수 (1 ~ 8)
        topping_numText.setFont(font);
        topping_numText.setString(topping_num[t_num]);
        topping_numText.setCharacterSize(100);
        topping_numText.setFillColor(Color::Red);
        topping_numText.setPosition(960.f, 470.f);
        ballonnText3.setFont(font);
        ballonnText3.setString(L"개 올려주세요!");
        ballonnText3.setCharacterSize(96);
        ballonnText3.setFillColor(Color::Black);
        ballonnText3.setPosition(1042.f, 474.f);
    }

    // 말풍선 페이드인 업데이트
    void update(float deltaTime) {
        if (balloonFadingIn) {
            if (balloonAlpha < 255) {
                balloonAlpha += 200 * deltaTime;
                if (balloonAlpha > 255) balloonAlpha = 255;
                balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(balloonAlpha)));
            }
        }
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(characterSprite);
        window.draw(balloonSprite);

        // 말풍선이 완전히 표시된 후에 텍스트를 그리기
        if (balloonAlpha >= 255) {
            window.draw(toppingText);
            window.draw(balloonText1);
            window.draw(flavorText);
            window.draw(ballonnText2);
            window.draw(topping_numText);
            window.draw(ballonnText3);
        }
    }

    void startBalloonFadeIn() {
        balloonFadingIn = true;
    }

private:
    int gender = 0;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture characterTexture;
    Sprite characterSprite;
    Texture balloonTexture;
    Sprite balloonSprite;
    float balloonAlpha = 0.0f;
    bool balloonFadingIn = false;
    Font font;              // 폰트 객체
    Text flavorText;        // 맛 (생크림, 초콜릿)
    Text toppingText;       // 토핑 종류 (딸기, 체리)
    Text topping_numText;   // 토핑갯수 (1 ~ 8)
    Text balloonText1;      // "케이크가 먹고싶어요!"
    Text ballonnText2;      // "케이크에"
    Text ballonnText3;      // "개 올려주세요!"
    const Vector2u windowSize = { 2880, 1800 };
};

int main() {
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");
    Clock clock;

    GameState gameState = GameState::StartScreen;

    StartScreen startScreen;
    OrderScreen orderScreen;

    bool characterShown = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (gameState == GameState::StartScreen && startScreen.isStartButtonPressed(mousePos)) {
                        gameState = GameState::OrderScreen;
                        characterShown = true;
                    }
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        window.clear();

        if (gameState == GameState::StartScreen) {
            startScreen.draw(window);
        }
        else if (gameState == GameState::OrderScreen) {
            orderScreen.draw(window);
            if (characterShown) {
                orderScreen.startBalloonFadeIn();
                characterShown = false;
            }
            orderScreen.update(deltaTime);
        }

        window.display();
    }

    return 0;
}
