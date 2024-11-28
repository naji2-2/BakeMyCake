#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <random>

using namespace sf;
using namespace std;

// 현재 화면의 상태
enum class GameState {
    StartScreen,
    OrderScreen,
    ChoseFlavorScreen
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

        // 시작화면 배경음악
        if (!startSoundBuffer.loadFromFile("Music/jazz-happy.mp3")) {
            cout << "Failed to load start sound" << endl;
        }
        startSound.setBuffer(startSoundBuffer);
        startSound.setLoop(true);   // 반복 재생
        startSound.setVolume(50);   // 볼륨 설정
        startSound.play();
    }

    void update(float deltaTime) {
        // StartScreen에서는 특별한 업데이트 로직이 없어 비워둠
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(buttonSprite);
    }

    bool isStartButtonPressed(Vector2i mousePos) {
        cout << "Pressed Start Button!" << endl;
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

        // 대답 버튼 초기화
        yesAnswerButton.setSize(Vector2f(823.f, 200.f));  // 크기
        yesAnswerButton.setPosition(1788.f, 1085.f);      // 위치
        yesAnswerButton.setFillColor(Color(0, 0, 0, 0));  // 투명한 배경

        noAnswerButton.setSize(Vector2f(823.f, 200.f));
        noAnswerButton.setPosition(1788.f, 1333.f);
        noAnswerButton.setFillColor(Color(0, 0, 0, 0));

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
        topping_numText.setPosition(970.f, 470.f);

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

        // 주문화면 배경음악
        if (!orderSoundBuffer.loadFromFile("Music/main.mp3")) {
            cout << "Failed to load order sound" << endl;
        }
        orderSound.setBuffer(orderSoundBuffer);
        orderSound.setLoop(true);
        orderSound.setVolume(50);
    }

    void update(float deltaTime) {

        // 주문 말풍선 페이드인
        if (oder_balloonFadingIn && oder_balloonAlpha < 255) {
            oder_balloonAlpha += 220 * deltaTime;
            if (oder_balloonAlpha > 255) {
                oder_balloonAlpha = 255;
                startAnswerBalloonFadeIn();
            }
            oder_balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(oder_balloonAlpha)));
        }

        // 대답 말풍선 페이드인
        if (answer_balloonFadingIn && answer_balloonAlpha < 255) {
            answer_balloonAlpha += 220 * deltaTime;
            if (answer_balloonAlpha > 255) answer_balloonAlpha = 255;
            answer_balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(answer_balloonAlpha)));
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
        }

        if (answer_balloonAlpha >= 255) {
            window.draw(yes_answer);
            window.draw(no_answer);
        }

    }

    void startBalloonFadeIn() {
        oder_balloonFadingIn = true;
    }

    void startAnswerBalloonFadeIn() {
        answer_balloonFadingIn = true;
    }

    bool isYesAnswerPressed(Vector2i mousePos) {
        return yesAnswerButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool isNoAnswerPressed(Vector2i mousePos) {
        return noAnswerButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
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
    RectangleShape yesAnswerButton; // "오케이" 버튼
    RectangleShape noAnswerButton;  // "나가주세요" 버튼
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

// 케이크 만들기 화면 클래스
class MakingCakeScreen {
public:
    MakingCakeScreen() {

        if (!backgroundTexture.loadFromFile("Images/Making_Seen.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        if (!cakeTexture.loadFromFile("Images/BaseCake.png")) {
            cout << "Failed to load BaseCake image" << endl;
        }
        cakeSprite.setTexture(cakeTexture);
        cakeSprite.setPosition(909.f, 724.f);

        // 선택지 버튼
        option1Button.setRadius(250.0f);
        option1Button.setPosition(200.f, 471.f);
        option1Button.setFillColor(Color(0, 0, 0, 0));

        option2Button.setRadius(250.0f);
        option2Button.setPosition(2180.f, 471.f);
        option2Button.setFillColor(Color(0, 0, 0, 0));

        // 이동 버튼
        moveLeftButton.setRadius(135.0f);
        moveLeftButton.setPosition(133.f, 1425.f);
        moveLeftButton.setFillColor(Color(0, 0, 0, 0));

        moveRightButton.setRadius(135.0f);
        moveRightButton.setPosition(2477.f, 1425.f);
        moveRightButton.setFillColor(Color(0, 0, 0, 0));

        // 이동 버튼 위치 확인용
        moveLeftButton.setOutlineThickness(2.f);
        moveLeftButton.setOutlineColor(Color::Green);
        moveRightButton.setOutlineThickness(2.f);
        moveRightButton.setOutlineColor(Color::Red);

        // 폰트 로드
        if (!font.loadFromFile("Fonts/JejuGothic.ttf")) {
            cout << "Failed to load font" << endl;
        }

        // 옵션 버튼
        if (!option1Texture.loadFromFile("Images/Option_Button.png")) {
            cout << "Failed to load option1 image" << endl;
        }
        option1Sprite.setTexture(option1Texture);
        option1Sprite.setPosition(200.f, 471.f);

        if (!option2Texture.loadFromFile("Images/Option_Button.png")) {
            cout << "Failed to load option2 image" << endl;
        }
        option2Sprite.setTexture(option1Texture);
        option2Sprite.setPosition(2180.f, 471.f);

        // 옵션 호버 버튼
        if (!option1hoverTexture.loadFromFile("Images/Option_Button_Hover.png")) {
            cout << "Failed to load option1hover image" << endl;
        }

        if (!option2hoverTexture.loadFromFile("Images/Option_Button_Hover.png")) {
            cout << "Failed to load option2hover image" << endl;
        }

        // 이동 버튼
        if (!moveLeftTexture.loadFromFile("Images/moveLeft_Button.png")) {
            cout << "Failed to load moveLeft image" << endl;
        }
        moveLeftSprite.setTexture(moveLeftTexture);
        moveLeftSprite.setPosition(133.f, 1425.f);

        if (!moveRightTexture.loadFromFile("Images/moveRight_Button.png")) {
            cout << "Failed to load moveRight image" << endl;
        }
        moveRightSprite.setTexture(moveRightTexture);
        moveRightSprite.setPosition(2477.f, 1425.f);

        // 화면 텍스트
        screenText.setFont(font);
        screenText.setString(L"케이크 만들기 화면입니다!");
        screenText.setCharacterSize(100);
        screenText.setFillColor(Color::White);
        screenText.setPosition(900.f, 261.f);

    }

    void update(float deltaTime) {

    }

    void draw(RenderWindow& window) {

    }

protected:
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture cakeTexture;                // 기본 케이크
    Sprite cakeSprite;
    Texture option1Texture;             // 선택지 옵션
    Sprite option1Sprite;
    Texture option2Texture;
    Sprite option2Sprite;
    Texture option1hoverTexture;        // 선택지 옵션 호버
    Sprite option1hoverSprite;
    Texture option2hoverTexture;
    Sprite option2hoverSprite;
    Texture moveLeftTexture;            //  이동(왼쪽)
    Sprite moveLeftSprite;
    Texture moveRightTexture;           // 이동(오른쪽)
    Sprite moveRightSprite;
    CircleShape option1Button;          // 선택지 버튼
    CircleShape option2Button;
    CircleShape moveLeftButton;      // 오른쪽 이동 버튼
    CircleShape moveRightButton;     // 왼쪽 이동 버튼
    Text screenText;                    // 설명 텍스트
    Font font;                          // 폰트 객체
    const Vector2u windowSize = { 2880, 1800 };
};

// 맛 선택 화면
class ChoseFlavorScreen : MakingCakeScreen {
public:
    ChoseFlavorScreen() {

        if (!creamflavorTexture.loadFromFile("Images/CreamFlavor.png")) {
            cout << "Failed to load CreamFlavor image" << endl;
        }
        creamflavorSprite.setTexture(creamflavorTexture);
        creamflavorSprite.setPosition(305.f, 566.f);

        if (!chocolateflavorTexture.loadFromFile("Images/ChocolateFlavor.png")) {
            cout << "Failed to load ChocolateFlavor image" << endl;
        }
        chocolateflavorSprite.setTexture(chocolateflavorTexture);
        chocolateflavorSprite.setPosition(2283.f, 566.f);

        if (!creamflavorcakeTexture.loadFromFile("Images/CreamFlavorCake.png")) {
            cout << "Failed to load CreamFlavorCake image" << endl;
        }

        if (!chocolateflavorcakeTexture.loadFromFile("Images/ChocolateFlavorCake.png")) {
            cout << "Failed to load ChocolateFlavorCake image" << endl;
        }

        screenText.setString(L"맛을 골라주세요!");
        screenText.setPosition(1100.f, 261.f);

    }

    bool option1state = false;
    bool option2state = false;

    void update(float deltaTime) {

        // 크림 케이크 맛 선택
        if (option1state) {
            cakeSprite.setColor(Color(255, 255, 255, 0));
            chocolateflavorcakeSprite.setColor(Color(255, 255, 255, 0));
            option2hoverSprite.setColor(Color(255, 255, 255, 0));
            option1hoverSprite.setTexture(option1hoverTexture);
            option1hoverSprite.setPosition(120.f, 391.f);
            option1hoverSprite.setColor(Color(255, 255, 255, 255));
            creamflavorcakeSprite.setTexture(creamflavorcakeTexture);
            creamflavorcakeSprite.setPosition(909.f, 724.f);
            creamflavorcakeSprite.setColor(Color(255, 255, 255, 255));
            option1state = false;
            option2state = false;
        }

        // 초콜릿 케이크 맛 선택
        if (option2state) {
            cakeSprite.setColor(Color(255, 255, 255, 0));
            creamflavorcakeSprite.setColor(Color(255, 255, 255, 0));
            option1hoverSprite.setColor(Color(255, 255, 255, 0));
            option2hoverSprite.setTexture(option2hoverTexture);
            option2hoverSprite.setPosition(2100.f, 391.f);
            option2hoverSprite.setColor(Color(255, 255, 255, 255));
            chocolateflavorcakeSprite.setTexture(chocolateflavorcakeTexture);
            chocolateflavorcakeSprite.setPosition(909.f, 724.f);
            chocolateflavorcakeSprite.setColor(Color(255, 255, 255, 255));
            option2state = false;
            option1state = false;
        }

    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(cakeSprite);
        window.draw(option1Sprite);
        window.draw(option1Button);
        window.draw(option2Sprite);
        window.draw(option2Button);
        window.draw(option1hoverSprite);
        window.draw(option2hoverSprite);
        window.draw(chocolateflavorSprite);
        window.draw(creamflavorSprite);
        window.draw(creamflavorcakeSprite);
        window.draw(chocolateflavorcakeSprite);
        window.draw(moveLeftSprite);
        window.draw(moveRightSprite);
        window.draw(screenText);
    }

    bool option1ButtonPressed(Vector2i mousePos) {
        return option1Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool option2ButtonPressed(Vector2i mousePos) {
        return option2Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool moveLeftButtonPressed(Vector2i mousePos) {
        return moveLeftButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool moveRightButtonPressed(Vector2i mousePos) {
        return moveRightButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    Texture chocolateflavorTexture;         // 초콜릿 맛
    Sprite chocolateflavorSprite;
    Texture creamflavorTexture;             // 생크림 맛
    Sprite creamflavorSprite;
    Texture chocolateflavorcakeTexture;     // 초콜릿 케이크
    Sprite chocolateflavorcakeSprite;
    Texture creamflavorcakeTexture;         // 생크림 케이크
    Sprite creamflavorcakeSprite;
};

int main() {
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");
    Clock clock;

    GameState gameState = GameState::StartScreen;

    StartScreen startScreen;
    OrderScreen orderScreen;
    ChoseFlavorScreen choseflavorScreen;

    bool characterShown = false;
    bool option1 = false;
    bool option2 = false;
    bool moveLeft = false;
    bool moveRight = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);
                if (gameState == GameState::StartScreen && startScreen.isStartButtonPressed(mousePos)) {
                    startScreen.stopSound();
                    orderScreen.playSound();
                    gameState = GameState::OrderScreen;
                    characterShown = true;
                }
                if (gameState == GameState::OrderScreen && orderScreen.isYesAnswerPressed(mousePos)) {
                    // TODO: 주문을 수락한 경우
                    cout << "Pressed Oder Yes Button!" << endl;
                    gameState = GameState::ChoseFlavorScreen;
                }
                else if (gameState == GameState::OrderScreen && orderScreen.isNoAnswerPressed(mousePos)) {
                    // TODO: 주문을 거절한 경우
                    cout << "Pressed Oder No Button!" << endl;
                }
                if (gameState == GameState::ChoseFlavorScreen && choseflavorScreen.option1ButtonPressed(mousePos)) {
                    // TODO: 왼쪽 선택지를 누른경우
                    cout << "Pressed Option1 Button!" << endl;
                    option1 = true;
                }
                else if (gameState == GameState::ChoseFlavorScreen && choseflavorScreen.option2ButtonPressed(mousePos)) {
                    // TODO: 오른쪽 선택지를 누른경우
                    cout << "Pressed Option2 Button!" << endl;
                    option2 = true;
                }

            }
        }

        float deltaTime = clock.restart().asSeconds();      // 프레임 사이의 시간 차이를 계산해 애니메이션을 일정한 속도로 보이게함

        // 화면 상태에 따라 업데이트 및 드로우 호출
        if (gameState == GameState::StartScreen) {
            startScreen.update(deltaTime); // 현재는 필요 없지만 구조 일관성을 위해 호출
        }
        else if (gameState == GameState::OrderScreen) {
            if (characterShown) {
                orderScreen.startBalloonFadeIn();
                characterShown = false;
            }
            orderScreen.update(deltaTime);
        }
        else if (gameState == GameState::ChoseFlavorScreen) {
            if (option1) {
                choseflavorScreen.option1state = true;
                choseflavorScreen.update(deltaTime);
                option1 = false;
            }
            if (option2) {
                choseflavorScreen.option2state = true;
                choseflavorScreen.update(deltaTime);
                option2 = false;
            }
        }

        window.clear();

        if (gameState == GameState::StartScreen) {
            startScreen.draw(window);
        }
        else if (gameState == GameState::OrderScreen) {
            orderScreen.draw(window);
        }
        else if (gameState == GameState::ChoseFlavorScreen) {
            choseflavorScreen.draw(window);
        }

        window.display();
    }

    return 0;
}