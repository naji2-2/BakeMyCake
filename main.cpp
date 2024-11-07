﻿#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>

using namespace sf;
using namespace std;

// 현재 화면의 상태
enum class GameState {
    StartScreen,
    OrderScreen
};

wstring flavor[] = { L"생크림", L"초코" };
wstring topping[] = { L"딸기", L"체리" };
string topping_num[] = { "1", "2", "3", "4", "5", "6", "7", "8" };
int gender[] = { 1, 2 };
int random_flavor, random_topping, random_topping_num;
int customer = 1;

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

        if (!startSoundBuffer.loadFromFile("Music/jazz-happy.mp3")) {
            cout << "Failed to load start sound" << endl;
        }
        startSound.setBuffer(startSoundBuffer);
        startSound.setLoop(true);   // 반복 재생
        startSound.setVolume(50);   // 볼륨 설정
        startSound.play();
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(buttonSprite);
    }

    bool isStartButtonPressed(Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    void stopSound() {
        startSound.stop();
    }

private:
    Sprite buttonSprite;
    Texture buttonTexture;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    const Vector2u windowSize = { 2880, 1800 };
    SoundBuffer startSoundBuffer;
    Sound startSound; 
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

        random_device rd;   // 난수 생성 객체
        mt19937 gen(rd());
        uniform_int_distribution<int> two(0, 1);
        uniform_int_distribution<int> top(0, 7);

        random_gender = two(gen);
        random_flavor = two(gen);
        random_topping = two(gen);
        random_topping_num = top(gen);

        // 손님 이미지 (0: 남자, 1: 여자)
        if (random_gender == 0) {
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

        // 주문 말풍선 이미지
        if (!balloonTexture.loadFromFile("Images/Text_Balloon.png")) {
            cout << "Failed to load Text Balloon image" << endl;
        }
        oder_balloonSprite.setTexture(balloonTexture);
        oder_balloonSprite.setPosition(149.f, 136.f);
        oder_balloonSprite.setColor(Color(255, 255, 255, oder_balloonAlpha));

        // 대답 말풍선 이미지
        if (!answer_balloonTexture.loadFromFile("Images/Answer_Balloon.png")) {
            cout << "Failed to load Answer Balloon image" << endl;
        }
        answer_balloonSprite.setTexture(answer_balloonTexture);
        answer_balloonSprite.setPosition(1705.f, 900.f);
        answer_balloonSprite.setColor(Color(255, 255, 255, answer_balloonAlpha));
        
        // 폰트 로드
        if (!font.loadFromFile("Fonts/JejuGothic.ttf")) {
            cout << "Failed to load font" << endl;
        }

        // 토핑 (딸기, 체리)
        toppingText.setFont(font);
        toppingText.setString(topping[random_topping]);
        toppingText.setCharacterSize(100);          // 글자 크기 설정
        toppingText.setFillColor(Color::Red);       // 글자 색상
        toppingText.setPosition(260.f, 291.f);      // 텍스트 위치 설정

        text1.setFont(font);
        text1.setString(L"가 들어간 케이크가 먹고 싶어요!");
        text1.setCharacterSize(96);
        text1.setFillColor(Color::Black);
        text1.setPosition(445.f, 292.f);

        // 맛 (생크림, 초콜릿)
        flavorText.setFont(font);
        flavorText.setString(flavor[random_flavor]);
        flavorText.setCharacterSize(100);
        flavorText.setFillColor(Color::Blue);
        if (random_flavor == 0)
            flavorText.setPosition(300.f, 470.f);   // 생크림
        else
            flavorText.setPosition(400.f, 470.f);   // 초코

        text2.setFont(font);
        text2.setString(L" 케이크에 ");
        text2.setCharacterSize(96);
        text2.setFillColor(Color::Black);
        text2.setPosition(570.f, 474.f);

        // 토핑 갯수 (1 ~ 8)
        topping_numText.setFont(font);
        topping_numText.setString(topping_num[random_topping_num]);
        topping_numText.setCharacterSize(100);
        topping_numText.setFillColor(Color::Red);
        topping_numText.setPosition(960.f, 470.f);

        text3.setFont(font);
        text3.setString(L"개 올려주세요!");
        text3.setCharacterSize(96);
        text3.setFillColor(Color::Black);
        text3.setPosition(1042.f, 474.f);

        // 답변 (오케이, 나가주세요)
        yes_answer.setFont(font);
        yes_answer.setString(L"오케이");
        yes_answer.setCharacterSize(95);
        yes_answer.setFillColor(Color::Black);
        yes_answer.setPosition(2067.f, 1131.f);

        no_answer.setFont(font);
        no_answer.setString(L"나가주세요");
        no_answer.setCharacterSize(95);
        no_answer.setFillColor(Color::Black);
        no_answer.setPosition(1985.f, 1386.f);

        if (!orderSoundBuffer.loadFromFile("Music/main.mp3")) {
            cout << "Failed to load order sound" << endl;
        }
        orderSound.setBuffer(orderSoundBuffer);
        orderSound.setLoop(true);
        orderSound.setVolume(50);
    }

    // 주문 말풍선 페이드인 업데이트
    void oder_update(float deltaTime) {
        if (oder_balloonFadingIn) {
            if (oder_balloonAlpha < 255) {
                oder_balloonAlpha += 200 * deltaTime;
                if (oder_balloonAlpha > 255) oder_balloonAlpha = 255;
                oder_balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(oder_balloonAlpha)));
            }
        }
    }

    // 대답 말풍선 페이드인
    void answer_update(float deltaTime) {
        if (answer_balloonFadingIn) {
            if (answer_balloonAlpha < 255) {
                answer_balloonAlpha += 200 * deltaTime;
                if (answer_balloonAlpha > 255) answer_balloonAlpha = 255;
                answer_balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(answer_balloonAlpha)));
            }
        }
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(characterSprite);
        window.draw(oder_balloonSprite);
        window.draw(answer_balloonSprite);

        // 말풍선이 완전히 표시된 후 텍스트 그리기
        if (oder_balloonAlpha >= 255) {
            window.draw(toppingText);
            window.draw(text1);
            window.draw(flavorText);
            window.draw(text2);
            window.draw(topping_numText);
            window.draw(text3);
            startAnswerBalloonFadeIn();
        }

        if (answer_balloonAlpha >= 255) {
            window.draw(yes_answer);
            window.draw(no_answer);
            // TODO: 대답 말풍선 속에 버튼을 눌러 게임으로 들어가는 기능, 손님을 돌려보내는 기능 추가
        }

    }

    void startBalloonFadeIn() {
        oder_balloonFadingIn = true;
    }
    
    void startAnswerBalloonFadeIn() {
        answer_balloonFadingIn = true;
        answerDelayClock.restart();
    }

    void playSound() {
        orderSound.play();
    }

    void stopSound() {
        orderSound.stop();
    }

private:
    Clock answerDelayClock;
    int random_gender = 0;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture characterTexture;
    Sprite characterSprite;
    Texture balloonTexture;
    Sprite oder_balloonSprite;
    Texture answer_balloonTexture;
    Sprite answer_balloonSprite;
    Text answerText;
    float oder_balloonAlpha = 0.0f;
    float answer_balloonAlpha = 0.0f;
    bool oder_balloonFadingIn = false;
    bool answer_balloonFadingIn = false;
    Font font;                  // 폰트 객체
    Text flavorText;            // 맛 (생크림, 초콜릿)
    Text toppingText;           // 토핑 종류 (딸기, 체리)
    Text topping_numText;       // 토핑갯수 (1 ~ 8)
    Text text1;                 // "케이크가 먹고싶어요!"
    Text text2;                 // "케이크에"
    Text text3;                 // "개 올려주세요!"
    Text yes_answer;            // "오케이"
    Text no_answer;             // "나가주세요"
    SoundBuffer orderSoundBuffer;
    Sound orderSound;
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
                        startScreen.stopSound();
                        orderScreen.playSound();
                        gameState = GameState::OrderScreen;
                        characterShown = true;
                    }
            }
        }

        float deltaTime = clock.restart().asSeconds();      // 프레임 사이의 시간 차이를 계산해 애니메이션을 일정한 속도로 보이게함

        window.clear();

        if (gameState == GameState::StartScreen) {
            startScreen.draw(window);
        }
        else if (gameState == GameState::OrderScreen) {
            orderScreen.draw(window);
            if (characterShown) {
                orderScreen.startBalloonFadeIn();
                characterShown = false;     // 반복해서 페이드인 되는 것을 방지함
            }
            orderScreen.oder_update(deltaTime);
            orderScreen.answer_update(deltaTime);
        }

        window.display();
    }

    return 0;
}