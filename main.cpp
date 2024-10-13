#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

// ���� ���¸� ��Ÿ���� ������
enum class GameState { StartScreen, Playing };

// ����ȭ�� Ŭ����
class StartScreen {
public:
    StartScreen() {
        // ��� �̹��� �ε�
        if (!backgroundTexture.loadFromFile("Images/Start_Seen_N.png")) {
            std::cout << "Failed to load background image" << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // ��������Ʈ ũ�⸦ â ũ�⿡ �°� ����
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        // ��ư �̹��� �ε�
        if (!buttonTexture.loadFromFile("Images/Start_Button.png")) {
            std::cout << "Failed to load button image" << std::endl;
        }
        buttonSprite.setTexture(buttonTexture);
        buttonSprite.setPosition(1026.f, 1325.f); // ��ư �̹��� ��ġ ����
    }

    // ���� ȭ�� �׸���
    void draw(sf::RenderWindow& window) {
        window.draw(backgroundSprite);
        window.draw(buttonSprite); // ��ư �̹��� �׸���
    }

    // ��ư Ŭ�� ó��
    bool isStartButtonPressed(sf::Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

private:
    Sprite buttonSprite;                            // ��ư �̹��� ��������Ʈ
    Texture buttonTexture;                          // ��ư �̹��� �ؽ�ó
    Texture backgroundTexture;                      // ��� �̹��� �ؽ�ó
    Sprite backgroundSprite;                        // ��� �̹��� ��������Ʈ
    const Vector2u windowSize = { 2880, 1800 };     // â ũ��
};

int main()
{
    // â ����
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");

    // ����ȭ�� ��ü ����
    StartScreen startScreen;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // ���콺 Ŭ�� ó��
            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mousePos = Mouse::getPosition(window);
                    if (startScreen.isStartButtonPressed(mousePos)) {
                        std::cout << "Start Game Button Pressed!" << std::endl;
                        // ���� ���� ���� �߰�
                    }
                }
            }
        }

        window.clear();
        startScreen.draw(window); // ���� ȭ�� �׸���
        window.display();
    }

    return 0;
}