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
    OrderScreen,
    ChoseFlavorScreen,
    ChoseToppingScreen,
    ChoseToppingNumScreen,
    ResultScreen
};

// 케이크, 주문자의 정보를 가지고있는 배열
wstring flavor[] = { L"생크림", L"초콜릿" };
wstring topping[] = { L"딸기", L"체리" };
wstring topping_num[] = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8" };

// main 함수에서 update를 위해 필요한 변수
bool characterShown = false;
bool cream = false;
bool chocolate = false;
bool strawberry = false;
bool cherry = false;
bool moveLeft = false;
bool moveRight = false;

// 손님이 주문한 케이크를 판단하는 변수
bool check_order_flavor = false;
bool check_order_topping = false;
bool check_order_toppingNum = false;
int Score = 0;

// 사용자가 만든 케이크를 판단하는 변수
bool check_cream = false;
bool check_chocolate = false;
bool check_strawberry = false;
bool check_cherry = false;
int check_toppingNum = 1;

// 손님의 주문을 저장하는 구조체
struct Order {
    int gender; // 0: 남자, 1: 여자
    int flavor; // 케이크 맛
    int topping; // 토핑 종류
    int toppingNum; // 토핑 개수
};

// 현재 주문을 저장하는 전역 변수
Order currentOrder;

// 게임 재시작을 위해 변수를 초기화하는 메서드
void resetGame(void) {
    characterShown = false;
    cream = false;
    chocolate = false;
    strawberry = false;
    cherry = false;
    check_cream = false;
    check_chocolate = false;
    check_strawberry = false;
    check_cherry = false;
    check_toppingNum = 1;
    check_order_flavor = false;
    check_order_topping = false;
    check_order_toppingNum = false;
    Score = 0;
}

// 텍스트 설정 메서드
void setText(Text& text, const wstring& content, int size, Color color, Vector2f position) {
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(position);
}

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

    void playSound() {
        startSound.play();
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
    OrderScreen() : gen(random_device{}()), two(0, 1), top(0, 7) {

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

        // 텍스트들의 폰트 초기화
        toppingText.setFont(font);
        text1.setFont(font);
        flavorText.setFont(font);
        text2.setFont(font);
        topping_numText.setFont(font);
        text3.setFont(font);
        yes_answer.setFont(font);
        no_answer.setFont(font);

        // 주문화면 배경음악
        if (!orderSoundBuffer.loadFromFile("Music/main.mp3")) {
            cout << "Failed to load order sound" << endl;
        }
        orderSound.setBuffer(orderSoundBuffer);
        orderSound.setLoop(true);
        orderSound.setVolume(50);
    }

    void new_order(void) {

        // 말풍선 투명도 초기화
        oder_balloonAlpha = 0.0f;
        answer_balloonAlpha = 0.0f;

        // 텍스트를 투명하게 바꿈
        toppingText.setFillColor(Color(255, 255, 255, 0));
        text1.setFillColor(Color(255, 255, 255, 0));
        flavorText.setFillColor(Color(255, 255, 255, 0));
        text2.setFillColor(Color(255, 255, 255, 0));
        topping_numText.setFillColor(Color(255, 255, 255, 0));
        text3.setFillColor(Color(255, 255, 255, 0));
        yes_answer.setFillColor(Color(255, 255, 255, 0));
        no_answer.setFillColor(Color(255, 255, 255, 0));

        // 손님의 정보와, 주문받을 케이크의 정보를 랜덤으로 설정
        currentOrder.gender = two(gen);
        currentOrder.flavor = two(gen);
        currentOrder.topping = two(gen);
        currentOrder.toppingNum = top(gen);

        // 손님 이미지를 랜덤으로 설정 (0: 남자, 1: 여자)
        if (currentOrder.gender == 0) {
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
            characterSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(oder_balloonAlpha)));
        }

        // 말풍선이 완전히 표시된 후 텍스트 그리기
        if (oder_balloonAlpha >= 255) {
            // 토핑 (딸기, 체리)
            setText(toppingText, topping[currentOrder.topping], 100, Color::Red, { 260.f, 291.f });
            setText(text1, L"가 들어간 케이크가 먹고 싶어요!", 96, Color::Black, { 445.f, 292.f });

            // 맛 (생크림, 초콜릿)
            setText(flavorText, flavor[currentOrder.flavor], 100, Color::Blue, { 300.f, 470.f });
            setText(text2, L" 케이크에 ", 96, Color::Black, { 570.f, 474.f });

            // 토핑 갯수 (1 ~ 8)
            setText(topping_numText, topping_num[currentOrder.toppingNum], 100, Color::Red, { 970.f, 470.f });
            setText(text3, L"개 올려주세요!", 96, Color::Black, { 1042.f, 474.f });
        }

        // 대답 말풍선 페이드인
        if (answer_balloonFadingIn && answer_balloonAlpha < 255) {
            answer_balloonAlpha += 220 * deltaTime;
            if (answer_balloonAlpha > 255) answer_balloonAlpha = 255;
            answer_balloonSprite.setColor(Color(255, 255, 255, static_cast<Uint8>(answer_balloonAlpha)));
        }

        if (answer_balloonAlpha >= 255) {
            // 답변 (오케이, 나가주세요)
            setText(yes_answer, L"오케이", 95, Color::Black, { 2067.f, 1131.f });
            setText(no_answer, L"나가주세요", 95, Color::Black, { 1985.f, 1386.f });
        }

    }

    void draw(RenderWindow& window) {

        window.draw(backgroundSprite);
        window.draw(characterSprite);
        window.draw(oder_balloonSprite);
        window.draw(answer_balloonSprite);
        window.draw(toppingText);
        window.draw(text1);
        window.draw(flavorText);
        window.draw(text2);
        window.draw(topping_numText);
        window.draw(text3);
        window.draw(yes_answer);
        window.draw(no_answer);

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
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture characterTexture;
    Sprite characterSprite;
    Texture balloonTexture;
    Sprite oder_balloonSprite;
    Texture answer_balloonTexture;
    Sprite answer_balloonSprite;
    Text answerText;
    // 난수 관련 변수
    mt19937 gen;
    uniform_int_distribution<int> two;
    uniform_int_distribution<int> top;
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
        setText(screenText, L"케이크 만들기 화면입니다!", 100, Color::White, { 900.f, 261.f });
    }

    // 케이크 상태 초기화
    virtual void new_cake() {

        // 옵션 선택 이벤트 초기화
        option1hoverSprite.setColor(Color(255, 255, 255, 0));
        option2hoverSprite.setColor(Color(255, 255, 255, 0));

    }

    // 옵션1 선택 이벤트
    void option1hover(void) {
        option2hoverSprite.setColor(Color(255, 255, 255, 0));
        option1hoverSprite.setTexture(option1hoverTexture);
        option1hoverSprite.setPosition(120.f, 391.f);
        option1hoverSprite.setColor(Color(255, 255, 255, 255));
    }
    // 옵션2 선택 이벤트
    void option2hover(void) {
        option1hoverSprite.setColor(Color(255, 255, 255, 0));
        option2hoverSprite.setTexture(option2hoverTexture);
        option2hoverSprite.setPosition(2100.f, 391.f);
        option2hoverSprite.setColor(Color(255, 255, 255, 255));
    }

    void update(float deltaTime) {

    }

    void draw(RenderWindow& window) {

    }

    bool moveLeftButtonPressed(Vector2i mousePos) {
        return moveLeftButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool moveRightButtonPressed(Vector2i mousePos) {
        return moveRightButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
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
class ChoseFlavorScreen :  public MakingCakeScreen {
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

    void new_cake() override {

        // 옵션 선택 이벤트 초기화
        option1hoverSprite.setColor(Color(255, 255, 255, 0));
        option2hoverSprite.setColor(Color(255, 255, 255, 0));

        // 케이크 이미지 투명도 변경
        creamflavorcakeSprite.setColor(Color(255, 255, 255, 0));
        chocolateflavorcakeSprite.setColor(Color(255, 255, 255, 0));

        // 기본 케이크 시트 보이기
        cakeSprite.setColor(Color(255, 255, 255, 255));

    }

    // 생크림 케이크 선택 이벤트
    void creamcake(void) {
        chocolateflavorcakeSprite.setColor(Color(255, 255, 255, 0));
        creamflavorcakeSprite.setTexture(creamflavorcakeTexture);
        creamflavorcakeSprite.setPosition(909.f, 724.f);
        creamflavorcakeSprite.setColor(Color(255, 255, 255, 255));
    }
    // 초콜릿 케이크 선택 이벤트
    void chocolatecake(void) {
        creamflavorcakeSprite.setColor(Color(255, 255, 255, 0));
        chocolateflavorcakeSprite.setTexture(chocolateflavorcakeTexture);
        chocolateflavorcakeSprite.setPosition(909.f, 724.f);
        chocolateflavorcakeSprite.setColor(Color(255, 255, 255, 255));
    }

    void update(float deltaTime) {

        // 크림 케이크 맛 선택
        if (check_cream) {
            cakeSprite.setColor(Color(255, 255, 255, 0));
            option1hover();
            creamcake();
        }

        // 초콜릿 케이크 맛 선택
        if (check_chocolate) {
            cakeSprite.setColor(Color(255, 255, 255, 0));
            option2hover();
            chocolatecake();
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
        //window.draw(moveLeftSprite);
        window.draw(moveRightSprite);
        window.draw(screenText);
    }

    bool creamflavorcakeButtonPressed(Vector2i mousePos) {
        return option1Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool chocolateflavorcakeButtonPressed(Vector2i mousePos) {
        return option2Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

protected:
    Texture chocolateflavorTexture;         // 초콜릿 맛
    Sprite chocolateflavorSprite;
    Texture creamflavorTexture;             // 생크림 맛
    Sprite creamflavorSprite;
    Texture chocolateflavorcakeTexture;     // 초콜릿 케이크
    Sprite chocolateflavorcakeSprite;
    Texture creamflavorcakeTexture;         // 생크림 케이크
    Sprite creamflavorcakeSprite;
};

// 토핑 선택 화면
class ChoseToppingScreen : public ChoseFlavorScreen {
public:
    ChoseToppingScreen() {

        if (!strawberrytoppingButtonTexture.loadFromFile("Images/Strawberry.png")) {
            cout << "Failed to load StrawberryToppingButton image" << endl;
        }
        strawberrytoppingButtonSprite.setTexture(strawberrytoppingButtonTexture);
        strawberrytoppingButtonSprite.setPosition(306.f, 566.f);

        if (!cherrytoppingButtonTexture.loadFromFile("Images/Cherry.png")) {
            cout << "Failed to load CherryToppingButton image" << endl;
        }
        cherrytoppingButtonSprite.setTexture(cherrytoppingButtonTexture);
        cherrytoppingButtonSprite.setPosition(2286.f, 566.f);

        if (!strawberrytoppingTexture.loadFromFile("Images/SmallStrawberry.png")) {
            cout << "Failed to load StrawberryTopping image" << endl;
        }

        if (!cherrytoppingTexture.loadFromFile("Images/SmallCherry.png")) {
            cout << "Failed to load CherryTopping image" << endl;
        }

        MakingCakeScreen::screenText.setString(L"토핑을 골라주세요!");
        screenText.setPosition(1025.f, 261.f);

    }

    // 딸기 토핑 선택 이벤트
    void strawberry(void) {
        cherrytoppingSprite.setColor(Color(255, 255, 255, 0));
        strawberrytoppingSprite.setTexture(strawberrytoppingTexture);
        strawberrytoppingSprite.setPosition(1360.f, 899.f);
        strawberrytoppingSprite.setColor(Color(255, 255, 255, 255));
    }
    // 체리 토핑 선택 이벤트
    void cherry(void) {
        strawberrytoppingSprite.setColor(Color(255, 255, 255, 0));
        cherrytoppingSprite.setTexture(cherrytoppingTexture);
        cherrytoppingSprite.setPosition(1360.f, 899.f);
        cherrytoppingSprite.setColor(Color(255, 255, 255, 255));
    }

    void new_cake() override {

        // 옵션 선택 이벤트 초기화
        option1hoverSprite.setColor(Color(255, 255, 255, 0));
        option2hoverSprite.setColor(Color(255, 255, 255, 0));

        // 토핑 이미지 투명도 변경
        strawberrytoppingSprite.setColor(Color(255, 255, 255, 0));
        cherrytoppingSprite.setColor(Color(255, 255, 255, 0));

        // 지정했던 맛의 케이크 보이게 하기
        if (check_cream) creamcake();
        if (check_chocolate) chocolatecake();

    }

    void update(float deltaTime) {
        
        // 부모 클래스에서 선택했던 케이크 상황으로 설정
         if (check_cream) {
             creamcake();   // 생크림 맛 케이크 이미지 띄우기
            // 토핑 선택
            if (check_strawberry) {
                // 딸기 토핑을 선택한 경우
                option1hover();
                strawberry();
            }
            if (check_cherry) {
                // 체리 토핑을 선택한 경우
                option2hover();
                cherry();
            }
         }
         if (check_chocolate) {
             chocolatecake();   // 초콜릿맛 케이크 이미지 띄우기
             // 토핑 선택
             if (check_strawberry) {
                 // 딸기 토핑을 선택한 경우
                 option1hover();
                 strawberry();
             }
             if (check_cherry) {
                 // 체리 토핑을 선택한 경우
                 option2hover();
                 cherry();
             }
         }
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(option1Sprite);
        window.draw(option1Button);
        window.draw(option2Sprite);
        window.draw(option2Button);
        window.draw(option1hoverSprite);
        window.draw(option2hoverSprite);
        window.draw(creamflavorcakeSprite);             // 생크림 케이크
        window.draw(chocolateflavorcakeSprite);         // 초콜릿 케이크
        window.draw(strawberrytoppingButtonSprite);     // 딸기 (버튼에 들어갈 이미지)
        window.draw(cherrytoppingButtonSprite);         // 체리 (버튼에 들어갈 이미지)
        window.draw(strawberrytoppingSprite);           // 토핑 딸기
        window.draw(cherrytoppingSprite);               // 토필 체리
        window.draw(moveLeftSprite);
        window.draw(moveRightSprite);
        window.draw(screenText);
    }

    bool strawberryButtonPressed(Vector2i mousePos) {
        return option1Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool cherryButtonPressed(Vector2i mousePos) {
        return option2Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }


protected:
    Texture strawberrytoppingButtonTexture;
    Sprite strawberrytoppingButtonSprite;
    Texture cherrytoppingButtonTexture;
    Sprite cherrytoppingButtonSprite;
    Texture strawberrytoppingTexture;
    Sprite strawberrytoppingSprite;
    Texture cherrytoppingTexture;
    Sprite cherrytoppingSprite;
};

// 토핑 갯수 선택 화면
class ChoseToppingNumScreen : public ChoseToppingScreen {
public:
    ChoseToppingNumScreen() {

        if (!toppingplusButtonTexture.loadFromFile("Images/Topping_Plus.png")) {
            cout << "Failed to load Topping Plus image" << endl;
        }
        toppingplusButtonSprite.setTexture(toppingplusButtonTexture);
        toppingplusButtonSprite.setPosition(2305.f, 596.f);
        
        if (!toppingminusButtonTexture.loadFromFile("Images/Topping_Minus.png")) {
            cout << "Failed to load Topping Minus image" << endl;
        }
        toppingminusButtonSprite.setTexture(toppingminusButtonTexture);
        toppingminusButtonSprite.setPosition(323.f, 596.f);

        // 딸기 토핑 이미지 초기화
        for (int i = 0; i < 8; i++) {
            strawberrytoppingSprite[i].setTexture(strawberrytoppingTexture);
        }

        // 체리 토핑 이미지 초기화
        for (int i = 0; i < 8; i++) {
            cherrytoppingSprite[i].setTexture(cherrytoppingTexture);
        }

        MakingCakeScreen::screenText.setString(L"토핑을 갯수를 정해주세요!");
        screenText.setPosition(880.f, 261.f);

    }

    // 토핑 수를 줄일때 필요없는 토핑 이미지의 투명도 조정
    void removetopping(int toppingnum) {
        for (int i = toppingnum; i < 8; i++) {
            if(check_strawberry) strawberrytoppingSprite[i].setColor(Color(255, 255, 255, 0));
            if(check_cherry) cherrytoppingSprite[i].setColor(Color(255, 255, 255, 0));
        }
    }
    
    // 토핑 갯수에 맞게 이미지를 띄우기
    void toppingNum(void) {
        switch (check_toppingNum)
        {
        case 1:
            if (check_strawberry) {
                removetopping(1);
                strawberrytoppingSprite[0].setPosition(1360.f, 899.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(1);
                cherrytoppingSprite[0].setPosition(1360.f, 899.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 2:
            if (check_strawberry) {
                removetopping(2);
                strawberrytoppingSprite[0].setPosition(1262.f, 899.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1471.f, 899.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(2);
                cherrytoppingSprite[0].setPosition(1262.f, 899.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1471.f, 899.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 3:
            if (check_strawberry) {
                removetopping(3);
                strawberrytoppingSprite[0].setPosition(1160.f, 899.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1369.f, 899.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1578.f, 899.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(3);
                cherrytoppingSprite[0].setPosition(1160.f, 899.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1369.f, 899.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1578.f, 899.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 4:
            if (check_strawberry) {
                removetopping(4);
                strawberrytoppingSprite[0].setPosition(1004.f, 899.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1369.f, 738.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1360.f, 1044.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[3].setPosition(1705.f, 899.f);
                strawberrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(4);
                cherrytoppingSprite[0].setPosition(1004.f, 899.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1369.f, 738.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1360.f, 1044.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[3].setPosition(1705.f, 899.f);
                cherrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 5:
            if (check_strawberry) {
                removetopping(5);
                strawberrytoppingSprite[0].setPosition(1083.f, 828.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1360.f, 756.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1655.f, 828.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[3].setPosition(1504.f, 1010.f);
                strawberrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[4].setPosition(1216.f, 1010.f);
                strawberrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(5);
                cherrytoppingSprite[0].setPosition(1083.f, 828.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1360.f, 756.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1655.f, 828.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[3].setPosition(1504.f, 1010.f);
                cherrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[4].setPosition(1216.f, 1010.f);
                cherrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 6:
            if (check_strawberry) {
                removetopping(6);
                strawberrytoppingSprite[0].setPosition(1098.f, 782.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1360.f, 721.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1622.f, 756.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[3].setPosition(1666.f, 971.f);
                strawberrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[4].setPosition(1360.f, 1044.f);
                strawberrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[5].setPosition(1072.f, 971.f);
                strawberrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(6);
                cherrytoppingSprite[0].setPosition(1098.f, 782.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1360.f, 721.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1622.f, 756.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[3].setPosition(1666.f, 971.f);
                cherrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[4].setPosition(1360.f, 1044.f);
                cherrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[5].setPosition(1072.f, 971.f);;
                cherrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 7:
            if (check_strawberry) {
                removetopping(7);
                strawberrytoppingSprite[0].setPosition(1103.f, 782.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1367.f, 736.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1631.f, 782.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[3].setPosition(1727.f, 963.f);
                strawberrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[4].setPosition(1487.f, 1045.f);
                strawberrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[5].setPosition(1247.f, 1045.f);
                strawberrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[6].setPosition(1007.f, 963.f);
                strawberrytoppingSprite[6].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(7);
                cherrytoppingSprite[0].setPosition(1103.f, 782.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1367.f, 736.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1631.f, 782.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[3].setPosition(1727.f, 963.f);
                cherrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[4].setPosition(1487.f, 1045.f);
                cherrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[5].setPosition(1247.f, 1045.f);
                cherrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[6].setPosition(1007.f, 963.f);
                cherrytoppingSprite[6].setColor(Color(255, 255, 255, 255));
            }
            break;
        case 8:
            if (check_strawberry) {
                removetopping(8);
                strawberrytoppingSprite[0].setPosition(1010.f, 901.f);
                strawberrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[1].setPosition(1154.f, 756.f);
                strawberrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[2].setPosition(1369.f, 756.f);
                strawberrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[3].setPosition(1584.f, 756.f);
                strawberrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[4].setPosition(1584.f, 1045.f);
                strawberrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[5].setPosition(1369.f, 1071.f);
                strawberrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[6].setPosition(1154.f, 1045.f);
                strawberrytoppingSprite[6].setColor(Color(255, 255, 255, 255));
                strawberrytoppingSprite[7].setPosition(1728.f, 901.f);
                strawberrytoppingSprite[7].setColor(Color(255, 255, 255, 255));
            }
            if (check_cherry) {
                removetopping(8);
                cherrytoppingSprite[0].setPosition(1010.f, 901.f);
                cherrytoppingSprite[0].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[1].setPosition(1154.f, 756.f);
                cherrytoppingSprite[1].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[2].setPosition(1369.f, 756.f);
                cherrytoppingSprite[2].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[3].setPosition(1584.f, 756.f);
                cherrytoppingSprite[3].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[4].setPosition(1584.f, 1045.f);
                cherrytoppingSprite[4].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[5].setPosition(1369.f, 1071.f);
                cherrytoppingSprite[5].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[6].setPosition(1154.f, 1045.f);
                cherrytoppingSprite[6].setColor(Color(255, 255, 255, 255));
                cherrytoppingSprite[7].setPosition(1728.f, 901.f);
                cherrytoppingSprite[7].setColor(Color(255, 255, 255, 255));
            }
            break;
        default:
            break;
        }
    }

    void update(float deltaTime) {

        // 부모 클래스에서 선택했던 케이크 상황으로 설정
        if (check_cream) {
            creamcake();    // 생크림 맛 케이크 이미지 띄우기
            // 토핑 선택
            if (check_strawberry) {
                // 딸기 토핑을 선택한 경우
                toppingNum();
            }
            else if (check_cherry) {
                // 체리 토핑을 선택한 경우
                toppingNum();
            }
        }
        if (check_chocolate) {
            // 초콜릿 케이크를 선택한 경우
            chocolatecake();    // 초콜릿맛 케이크 이미지 띄우기
            // 토핑 선택
            if (check_strawberry) {
                // 딸기 토핑을 선택한 경우
                toppingNum();
            }
            else if (check_cherry) {
                // 체리 토핑을 선택한 경우
                toppingNum();
            }
        }
    }

    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(option1Sprite);
        window.draw(option1Button);
        window.draw(option2Sprite);
        window.draw(option2Button);
        window.draw(toppingplusButtonSprite);
        window.draw(toppingminusButtonSprite);
        window.draw(creamflavorcakeSprite);             // 생크림 케이크
        window.draw(chocolateflavorcakeSprite);         // 초콜릿 케이크
        for (int i = 0; i < 8; i++) {
            if(check_strawberry) window.draw(strawberrytoppingSprite[i]);    // 딸기 토핑
            if (check_cherry) window.draw(cherrytoppingSprite[i]);           // 체리 토핑
        }
        window.draw(moveLeftSprite);
        window.draw(moveRightSprite);
        window.draw(screenText);
    }

    bool minusButtonPressed(Vector2i mousePos) {
        return option1Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool plusButtonPressed(Vector2i mousePos) {
        return option2Button.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

protected:
    Texture toppingplusButtonTexture;
    Sprite toppingplusButtonSprite;
    Texture toppingminusButtonTexture;
    Sprite toppingminusButtonSprite;
    // 딸기 토핑 8개 스프라이트
    Sprite strawberrytoppingSprite[8];
    // 체리 토핑 8개 스프라이트
    Sprite cherrytoppingSprite[8];

};

// 결과 화면
class ResultScreen {
public:
    ResultScreen() {
        if (!backgroundTexture.loadFromFile("Images/Order_Seen.png")) {
            cout << "Failed to load background image" << endl;
        }

        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        // 손님 이미지 (0: 남자, 1: 여자)
        if (currentOrder.gender == 0) {
            if (!characterTexture.loadFromFile("Images/male.png")) {
                cout << "Failed to load male character image" << endl;
            }
        }
        else {
            if (!characterTexture.loadFromFile("Images/female.png")) {
                cout << "Failed to load female character image" << endl;
            }
        }

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

    }

    // 케이크의 점수 지정
    void sumScore(void) {
        if (check_order_toppingNum)
            // 토핑 갯수만 일치하는 경우
            Score = 20;
        if (check_order_flavor || check_order_topping)
            // 맛이나 토핑이 일치하는 경우
            Score = 40;
        if ((check_order_flavor && check_order_toppingNum) || (check_order_topping && check_order_toppingNum))
            // 맛과 토핑갯수가 일치하거나 토핑 종류와 갯수가 일치하는 경우
            Score = 60;
        if (check_order_flavor && check_order_topping)
            // 맛과 토핑이 일치하는 경우
            Score = 80;
        if (check_order_flavor && check_order_topping && check_order_toppingNum)
            // 주문을 정확이 만든 경우
            Score = 100;
    }

    void new_result(void) {

        // 말풍선 투명도 초기화
        oder_balloonAlpha = 0.0f;
        answer_balloonAlpha = 0.0f;

        // 텍스트를 투명하게 바꿈
        toppingText.setFillColor(Color(255, 255, 255, 0));
        flavorText.setFillColor(Color(255, 255, 255, 0));
        topping_numText.setFillColor(Color(255, 255, 255, 0));
        score.setFillColor(Color(255, 255, 255, 0));
        text1.setFillColor(Color(255, 255, 255, 0));
        text2.setFillColor(Color(255, 255, 255, 0));
        text3.setFillColor(Color(255, 255, 255, 0));
        text4.setFillColor(Color(255, 255, 255, 0));
        text5.setFillColor(Color(255, 255, 255, 0));
        again_answer.setFillColor(Color(255, 255, 255, 0));
        stop_answer.setFillColor(Color(255, 255, 255, 0));
    }

    void update(float deltaTime) {

        // 만든 케이크가 주문과 일치하는지 판별
        if ((flavor[currentOrder.flavor] == flavor[0] && check_cream) || (flavor[currentOrder.flavor] == flavor[1] && check_chocolate)) {
            // 맛이 일치
            check_order_flavor = true;
        }
        if ((currentOrder.topping == 0 && check_strawberry) || (currentOrder.topping == 1 && check_cherry)) {
            // 토핑이 일치
            check_order_topping = true;
        }
        if (stoi(topping_num[currentOrder.toppingNum]) == check_toppingNum) {
            // 토핑 갯수가 일치
            check_order_toppingNum = true;
        }

        // 손님 이미지를 랜덤으로 설정 (0: 남자, 1: 여자)
        if (currentOrder.gender == 0) {
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

        // 텍스트 폰트 초기화
        flavorText.setFont(font);
        toppingText.setFont(font);
        topping_numText.setFont(font);
        score.setFont(font);
        text1.setFont(font);
        text2.setFont(font);
        text3.setFont(font);
        text4.setFont(font);
        text5.setFont(font);

        // 손님 말풍선이 다 나온다음
        if ((oder_balloonAlpha >= 255)) {
            sumScore();
            setText(text4, L"이 케이크는", 96, Color::Black, { 461.f, 550.f });

            // 손님의 주문과 일치여부에 따라 텍스트 설정 
            if (check_order_flavor) {
                // 케이크 맛이 일치하는 경우
                setText(flavorText, flavor[currentOrder.flavor], 100, Color::Blue, { 566.f, 255.f });
                setText(text1, L"맛 케이크에", 96, Color::Black, { 832.f, 255.f });
            }
            else {
                // 케이크 맛이 일치하지 않는 경우
                setText(flavorText, flavor[currentOrder.flavor], 100, Color::Blue, { 431.f, 255.f });
                setText(text1, L"맛 케이크가 아니고...", 96, Color::Black, { 697.f, 255.f });
            }

            if (check_order_topping) {
                // 케이크 토핑만 일치하는 경우
                setText(toppingText, topping[currentOrder.topping], 100, Color::Red, { 419.f, 394.f });
                setText(text2, L"가", 96, Color::Black, { 599.f, 402.f });
                setText(topping_numText, topping_num[currentOrder.toppingNum], 100, Color::Red, { 713.f, 394.f });
                setText(text3, L"개 안 들어가있네요", 96, Color::Black, { 776.f, 402.f });
            }
            if (check_order_topping && check_order_toppingNum) {
                // 케이크 토핑과 갯수가 일치하는 경우
                setText(toppingText, topping[currentOrder.topping], 100, Color::Red, { 419.f, 394.f });
                setText(text2, L"가", 96, Color::Black, { 599.f, 402.f });
                setText(topping_numText, topping_num[currentOrder.toppingNum], 100, Color::Red, { 713.f, 394.f });
                setText(text3, L"개 잘 들어가있네요!", 96, Color::Black, { 776.f, 402.f });
            }
            else {
                // 주문한 토핑이 안 들어간 경우
                setText(toppingText, topping[currentOrder.topping], 100, Color::Red, { 419.f, 394.f });
                setText(text2, L"가", 96, Color::Black, { 599.f, 402.f });
                setText(topping_numText, topping_num[currentOrder.toppingNum], 100, Color::Red, { 713.f, 394.f });
                setText(text3, L"개 안 들어갔고", 96, Color::Black, { 776.f, 402.f });
            }

            // 점수
            if (Score == 100) {
                setText(score, to_wstring(Score), 100, Color::Magenta, { 954.f, 550.f });
                setText(text5, L"점이에요!", 96, Color::Black, { 1131.f, 550.f });
            }
            else if (Score == 0) {
                setText(score, to_wstring(Score), 100, Color::Magenta, { 1009.f, 550.f });
                setText(text5, L"점이에요!", 96, Color::Black, { 1080.f, 550.f });
            }
            else {
                setText(score, to_wstring(Score), 100, Color::Magenta, { 961.f, 550.f });
                setText(text5, L"점이에요!", 96, Color::Black, { 1091.f, 550.f });
            }

        }

        // 답변 말풍선이 다 나온 다음
        if (answer_balloonAlpha >= 255) {
            // 대답 버튼 초기화
            againButton.setSize(Vector2f(823.f, 200.f));  // 크기
            againButton.setPosition(1788.f, 1085.f);      // 위치
            againButton.setFillColor(Color(0, 0, 0, 0));  // 투명한 배경
            stopButton.setSize(Vector2f(823.f, 200.f));
            stopButton.setPosition(1788.f, 1333.f);
            stopButton.setFillColor(Color(0, 0, 0, 0));
            // 답변 (다시하기, 그만하기)
            again_answer.setFont(font);
            stop_answer.setFont(font);
            setText(again_answer, L"다시하기", 95, Color::Black, { 2010.f, 1131.f });
            setText(stop_answer, L"그만하기", 95, Color::Black, { 2010.f, 1386.f });
        }

    }

    void draw(RenderWindow& window) {

        window.draw(backgroundSprite);
        window.draw(characterSprite);
        window.draw(oder_balloonSprite);
        window.draw(answer_balloonSprite);
        window.draw(toppingText);
        window.draw(flavorText);
        window.draw(topping_numText);
        window.draw(score);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);
        window.draw(text5);
        window.draw(again_answer);
        window.draw(stop_answer);

    }

    void startBalloonFadeIn() {
        oder_balloonFadingIn = true;
    }

   void startAnswerBalloonFadeIn() {
        answer_balloonFadingIn = true;
    }

    bool AgainButtonPressed(Vector2i mousePos) {
        return againButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

    bool StopButtonPressed(Vector2i mousePos) {
        return stopButton.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    Clock answerDelayClock;
    Texture backgroundTexture;
    Sprite backgroundSprite;
    Texture characterTexture;
    Sprite characterSprite;
    Texture balloonTexture;
    Sprite oder_balloonSprite;
    Texture answer_balloonTexture;
    Sprite answer_balloonSprite;
    Text againText;
    RectangleShape againButton; // "다시하기" 버튼
    RectangleShape stopButton;  // "그만하기" 버튼
    float oder_balloonAlpha = 0.0f;
    float answer_balloonAlpha = 0.0f;
    bool oder_balloonFadingIn = false;
    bool answer_balloonFadingIn = false;
    Font font;                  // 폰트 객체
    Text flavorText;            // 맛 (생크림, 초콜릿)
    Text toppingText;           // 토핑 종류 (딸기, 체리)
    Text topping_numText;       // 토핑갯수 (1 ~ 8)
    Text score;                 // 점수 (20, 40, 60, 80, 100)
    Text text1;                 // "맛 케이크에" or "맛 케이크가 아니고..."
    Text text2;                 // "가"
    Text text3;                 // "개 잘 들어가있네요!" or "개 안 들어가있네요"
    Text text4;                 // "이 케이크는"
    Text text5;                 // "점이에요!"
    Text again_answer;          // "다시하기"
    Text stop_answer;           // "그만하기"
    const Vector2u windowSize = { 2880, 1800 };
};

int main() {
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");
    Clock clock;

    GameState gameState = GameState::StartScreen;

    StartScreen startScreen;
    OrderScreen orderScreen;
    MakingCakeScreen makingScreen;
    ChoseFlavorScreen choseflavorScreen;
    ChoseToppingScreen chosetoppingScreen;
    ChoseToppingNumScreen chosetoppingnumScreen;
    ResultScreen resultScreen;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2i mousePos = Mouse::getPosition(window);

                switch (gameState)
                {
                case GameState::StartScreen:
                    if (startScreen.isStartButtonPressed(mousePos)) {
                        startScreen.stopSound();
                        orderScreen.playSound();
                        gameState = GameState::OrderScreen;
                        characterShown = true;
                        orderScreen.new_order();
                        choseflavorScreen.new_cake();
                    }
                    break;
                case GameState::OrderScreen:
                    if (orderScreen.isYesAnswerPressed(mousePos)) {
                        // 주문을 수락한 경우
                        cout << "Pressed Oder Yes Button!" << endl;
                        gameState = GameState::ChoseFlavorScreen;
                    }
                    else if ( orderScreen.isNoAnswerPressed(mousePos)) {
                        // 주문을 거절한 경우
                        cout << "Pressed Oder No Button!" << endl;
                        resetGame();
                        orderScreen.new_order();
                        choseflavorScreen.new_cake();
                    }
                    break;
                case GameState::ChoseFlavorScreen:
                    if (choseflavorScreen.creamflavorcakeButtonPressed(mousePos)) {
                        //  생크림 맛을 선택한 경우
                        cout << "Pressed creamflavorcake Button!" << endl;
                        cream = true;
                        chocolate = false;
                    }
                    else if (choseflavorScreen.chocolateflavorcakeButtonPressed(mousePos)) {
                        // 초콜릿 맛을 선택한 경우
                        cout << "Pressed chocolateflavorcake Button!" << endl;
                        chocolate = true;
                        cream = false;
                    }
                    if (makingScreen.moveRightButtonPressed(mousePos)) {
                        // 오른쪽 버튼을 이용해 화면을 전환한 경우
                        gameState = GameState::ChoseToppingScreen;
                        chosetoppingScreen.new_cake();
                    }
                    break;
                case GameState::ChoseToppingScreen:
                    if (chosetoppingScreen.strawberryButtonPressed(mousePos)) {
                        cout << "Pressed strawberry Button!" << endl;
                        cherry = false;
                        strawberry = true;
                    }
                    else if (chosetoppingScreen.cherryButtonPressed(mousePos)) {
                        cout << "Pressed cherry Button!" << endl;
                        strawberry = false;
                        cherry = true;
                    }
                    if (makingScreen.moveRightButtonPressed(mousePos))
                        // 오른쪽 버튼을 이용해 화면을 전환한 경우
                        gameState = GameState::ChoseToppingNumScreen;
                    if (makingScreen.moveLeftButtonPressed(mousePos))
                        // 왼쪽 버튼을 이용해 화면을 전환한 경우
                        gameState = GameState::ChoseFlavorScreen;
                    break;
                case GameState::ChoseToppingNumScreen:
                    if (chosetoppingnumScreen.plusButtonPressed(mousePos)) {
                        // 토핑 갯수를 증가시킴
                        if (check_toppingNum < 8)
                            check_toppingNum++;
                        cout << "ToppingNum : " << check_toppingNum << endl;
                    }
                    if (chosetoppingnumScreen.minusButtonPressed(mousePos)) {
                        // 토핑 갯수를 감소시킴
                        if (check_toppingNum > 1)
                            check_toppingNum--;
                        cout << "ToppingNum : " << check_toppingNum << endl;
                    }
                    if (makingScreen.moveRightButtonPressed(mousePos))
                        // 오른쪽 버튼을 이용해 화면을 전환한 경우
                        gameState = GameState::ResultScreen;
                        resultScreen.new_result();
                        characterShown = true;
                    if (makingScreen.moveLeftButtonPressed(mousePos))
                        // 왼쪽 버튼을 이용해 화면을 전환한 경우
                        gameState = GameState::ChoseToppingScreen;
                    break;
                case GameState::ResultScreen:
                    if (resultScreen.AgainButtonPressed(mousePos)) {
                        // 다시하기 버튼을 눌렀을 때 주문관련 변수를 초기화 시키고, 주문을 새로 받음
                        cout << "Pressed Again Button!" << check_toppingNum << endl;
                        resetGame();
                        orderScreen.new_order();
                        choseflavorScreen.new_cake();
                        gameState = GameState::OrderScreen;
                    }
                    else if (resultScreen.StopButtonPressed(mousePos)) {
                        // 그만하기 버튼을 눌렀을 때 주문관련 변수를 초기화 시키고, 시작화면으로 이동함
                        cout << "Pressed Stop Button!" << endl;
                        orderScreen.stopSound();
                        startScreen.playSound();
                        resetGame();
                        gameState = GameState::StartScreen;
                    }
                    break;
                default:
                    break;
                }

            }
        }

        float deltaTime = clock.restart().asSeconds();      // 프레임 사이의 시간 차이를 계산해 애니메이션을 일정한 속도로 보이게함

        // 화면 상태에 따라 업데이트 및 드로우 호출
        switch (gameState)
        {
        case GameState::StartScreen:
            startScreen.update(deltaTime);
            break;
        case GameState::OrderScreen:
            if (characterShown) {
                orderScreen.startBalloonFadeIn();
                characterShown = false;
            }
            orderScreen.update(deltaTime);
            break;
        case GameState::ChoseFlavorScreen:
            if (cream) {
                // 케이크 맛을 크림으로 변경
                check_chocolate = false;
                check_cream = true;
                // cream 초기화
                cream = false;
            }
            if (chocolate) {
                // 케이크 맛을 초콜릿으로 변경
                check_cream = false;
                check_chocolate = true;
                // chocolate 초기화
                chocolate = false;
            }
            choseflavorScreen.update(deltaTime);
            break;
        case GameState::ChoseToppingScreen:
            if (strawberry) {
                // 토핑 상태를 딸기로 변경
                check_cherry = false;
                check_strawberry = true;
                // strawberry 초기화
                strawberry = false;
            }
            if (cherry) {
                // 토핑 상태를 체리로 변경
                check_strawberry = false;
                check_cherry = true;
                // cherry 초기화
                cherry = false;
            }
            chosetoppingScreen.update(deltaTime);
            break;
        case GameState::ChoseToppingNumScreen:
            chosetoppingnumScreen.update(deltaTime);
            break;
        case GameState::ResultScreen:
            if (characterShown) {
                resultScreen.startBalloonFadeIn();
                characterShown = false;
            }
            resultScreen.update(deltaTime);
            break;
        default:
            break;
        }

        window.clear();

        switch (gameState)
        {
        case GameState::StartScreen:
            startScreen.draw(window);
            break;
        case GameState::OrderScreen:
            orderScreen.draw(window);
            break;
        case GameState::ChoseFlavorScreen:
            choseflavorScreen.draw(window);
            break;
        case GameState::ChoseToppingScreen:
            chosetoppingScreen.draw(window);
            break;
        case GameState::ChoseToppingNumScreen:
            chosetoppingnumScreen.draw(window);
            break;
        case GameState::ResultScreen:
            resultScreen.draw(window);
            break;
        default:
            break;
        }

        window.display();
    }

    return 0;
}