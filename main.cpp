#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

// 게임 상태를 나타내는 열거형
enum class GameState { StartScreen, Playing };

// 시작화면 클래스
class StartScreen {
public:
    StartScreen() {
        // 배경 이미지 로드
        if (!backgroundTexture.loadFromFile("Images/Start_Seen_N.png")) {
            std::cout << "Failed to load background image" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // 스프라이트 크기를 창 크기에 맞게 조정
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        // 버튼 이미지 로드
        if (!buttonTexture.loadFromFile("Images/Start_Button.png")) {
            std::cout << "Failed to load button image" << std::endl;
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
    bool isStartButtonPressed(sf::Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

private:
    Sprite buttonSprite;                            // 버튼 이미지 스프라이트
    Texture buttonTexture;                          // 버튼 이미지 텍스처
    Texture backgroundTexture;                      // 배경 이미지 텍스처
    Sprite backgroundSprite;                        // 배경 이미지 스프라이트
    const Vector2u windowSize = { 2880, 1800 };     // 창 크기
};

int main()
{
    // 창 생성
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");

    // 시작화면 객체 생성
    StartScreen startScreen;

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
                    if (startScreen.isStartButtonPressed(mousePos)) {
                        std::cout << "Start Game Button Pressed!" << std::endl;
                        // 게임 시작 로직 추가
                    }
                }
            }
        }

        window.clear();
        startScreen.draw(window); // 시작 화면 그리기
        window.display();
    }

    return 0;
}