#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

// 게임 상태를 나타내는 열거형
enum class GameState { StartScreen, OrderScreen };

int flavor = 0;
string flavor[] = {"생크림", "초코"};
int topping = 0;
string topping[] = {"딸기", "체리"};
int topping_num[] = {1, 2, 3, 4, 5, 6, 7, 8};
int gender[] = { 1, 2 };

// 시작화면 클래스
class StartScreen {
public:
    StartScreen() {
        // 배경 이미지 로드
        if (!backgroundTexture.loadFromFile("Images/Start_Seen_N.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // 스프라이트 크기를 창 크기에 맞게 조정
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        // 버튼 이미지 로드
        if (!buttonTexture.loadFromFile("Images/Start_Button.png")) {
            cout << "Failed to load button image" << endl;
        }
        buttonSprite.setTexture(buttonTexture);
        buttonSprite.setPosition(1026.f, 1325.f); // 버튼 이미지 위치 설정
    }

    // 시작 화면 그리기
    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(buttonSprite); // 버튼 이미지 그리기
    }

    // 버튼 클릭 처리
    bool isStartButtonPressed(Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    Sprite buttonSprite;                            // 버튼 이미지 스프라이트
    Texture buttonTexture;                          // 버튼 이미지 텍스처
    Texture backgroundTexture;                      // 배경 이미지 텍스처
    Sprite backgroundSprite;                        // 배경 이미지 스프라이트
    const Vector2u windowSize = { 2880, 1800 };     // 창 크기
};

// 주문 화면 클래스
class OrderScreen {
public:
    OrderScreen() {
        // 이미지 로드
        if (!backgroundTexture.loadFromFile("Images/Order_Seen.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // 스프라이트 크기를 창 크기에 맞게 조정
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );
    }

    // 주문 화면 그리기
    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
    }

private:
    Texture backgroundTexture;                      // 배경 이미지 텍스처
    Sprite backgroundSprite;                        // 배경 이미지 스프라이트
    const Vector2u windowSize = { 2880, 1800 };     // 창 크기
};

int main()
{
    // 창 생성
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");

    // 게임 상태
    GameState gameState = GameState::StartScreen;

    // 시작화면 및 주문화면 객체 생성
    StartScreen startScreen;
    OrderScreen orderScreen;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // 마우스 클릭 처리
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);

                    // 게임 상태가 StartScreen일 때 버튼을 클릭하면 OrderScreen으로 상태 전환
                    if (gameState == GameState::StartScreen && startScreen.isStartButtonPressed(mousePos)) {
                        cout << "Start Game Button Pressed!" << endl;
                        gameState = GameState::OrderScreen;
                    }
                }
            }
        }

        window.clear();

        // 게임 상태에 따라 화면 그리기
        if (gameState == GameState::StartScreen) {
            startScreen.draw(window); // 시작 화면
        }
        else if (gameState == GameState::OrderScreen) {
            orderScreen.draw(window); // 주문 화면
        }

        window.display();
    }

    return 0;
}
