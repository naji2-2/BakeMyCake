#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

// ���� ���¸� ��Ÿ���� ������
enum class GameState { StartScreen, OrderScreen };

int flavor = 0;
string flavor[] = {"��ũ��", "����"};
int topping = 0;
string topping[] = {"����", "ü��"};
int topping_num[] = {1, 2, 3, 4, 5, 6, 7, 8};
int gender[] = { 1, 2 };

// ����ȭ�� Ŭ����
class StartScreen {
public:
    StartScreen() {
        // ��� �̹��� �ε�
        if (!backgroundTexture.loadFromFile("Images/Start_Seen_N.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // ��������Ʈ ũ�⸦ â ũ�⿡ �°� ����
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );

        // ��ư �̹��� �ε�
        if (!buttonTexture.loadFromFile("Images/Start_Button.png")) {
            cout << "Failed to load button image" << endl;
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
    bool isStartButtonPressed(Vector2i mousePos) {
        return buttonSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos));
    }

private:
    Sprite buttonSprite;                            // ��ư �̹��� ��������Ʈ
    Texture buttonTexture;                          // ��ư �̹��� �ؽ�ó
    Texture backgroundTexture;                      // ��� �̹��� �ؽ�ó
    Sprite backgroundSprite;                        // ��� �̹��� ��������Ʈ
    const Vector2u windowSize = { 2880, 1800 };     // â ũ��
};

// �ֹ� ȭ�� Ŭ����
class OrderScreen {
public:
    OrderScreen() {
        // �̹��� �ε�
        if (!backgroundTexture.loadFromFile("Images/Order_Seen.png")) {
            cout << "Failed to load background image" << endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        // ��������Ʈ ũ�⸦ â ũ�⿡ �°� ����
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
            static_cast<float>(windowSize.y) / backgroundTexture.getSize().y
        );
    }

    // �ֹ� ȭ�� �׸���
    void draw(RenderWindow& window) {
        window.draw(backgroundSprite);
    }

private:
    Texture backgroundTexture;                      // ��� �̹��� �ؽ�ó
    Sprite backgroundSprite;                        // ��� �̹��� ��������Ʈ
    const Vector2u windowSize = { 2880, 1800 };     // â ũ��
};

int main()
{
    // â ����
    RenderWindow window(VideoMode(2880, 1800), "Bake My Cake!");

    // ���� ����
    GameState gameState = GameState::StartScreen;

    // ����ȭ�� �� �ֹ�ȭ�� ��ü ����
    StartScreen startScreen;
    OrderScreen orderScreen;

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

                    // ���� ���°� StartScreen�� �� ��ư�� Ŭ���ϸ� OrderScreen���� ���� ��ȯ
                    if (gameState == GameState::StartScreen && startScreen.isStartButtonPressed(mousePos)) {
                        cout << "Start Game Button Pressed!" << endl;
                        gameState = GameState::OrderScreen;
                    }
                }
            }
        }

        window.clear();

        // ���� ���¿� ���� ȭ�� �׸���
        if (gameState == GameState::StartScreen) {
            startScreen.draw(window); // ���� ȭ��
        }
        else if (gameState == GameState::OrderScreen) {
            orderScreen.draw(window); // �ֹ� ȭ��
        }

        window.display();
    }

    return 0;
}
