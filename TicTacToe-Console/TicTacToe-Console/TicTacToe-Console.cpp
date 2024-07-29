#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

// Define the game board
char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

// Victory tracker
int winsX = 0;
int winsO = 0;

// Reset the game
void resetGame(char board[3][3], char& currentPlayer, bool& isRunning, sf::Text& messageText, sf::Text& winsXText, sf::Text& winsOText) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }
    currentPlayer = 'X';
    isRunning = true;
    messageText.setString("");
    winsXText.setString("");
    winsOText.setString("");
}

// Render the board
void renderBoard(sf::RenderWindow& window, sf::Sprite& boardSprite, sf::Sprite& xSprite, sf::Sprite& oSprite, sf::RectangleShape& messageBackground, sf::Text& messageText, sf::Text& winsXText, sf::Text& winsOText) {
    window.clear();
    window.draw(boardSprite);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == 'X') {
                xSprite.setPosition(11 + j * 207, 11 + i * 207);
                window.draw(xSprite);
            }
            else if (board[i][j] == 'O') {
                oSprite.setPosition(11 + j * 207, 11 + i * 207);
                window.draw(oSprite);
            }
        }
    }

    // Draw victory/draw message
    if (!messageText.getString().isEmpty()) {
        window.draw(messageBackground);
        window.draw(messageText);
        window.draw(winsXText);
        window.draw(winsOText);
    }

    window.display();
}

// Player move function
void playerMove(int row, int col, char player) {
    if (board[row][col] == ' ') {
        board[row][col] = player;
    }
}

// Check for win
bool checkWin(char player) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
            return true;
        }
    }
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
        return true;
    }
    return false;
}

// Check for draw
bool checkDraw() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return false;
            }
        }
    }
    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "Tic-Tac-Toe");

    // Load textures
    sf::Texture boardTexture, xTexture, oTexture;
    if (!boardTexture.loadFromFile("img/board.png") || !xTexture.loadFromFile("img/x.png") || !oTexture.loadFromFile("img/o.png")) {
        return -1;
    }

    // Create sprites and scale them
    sf::Sprite boardSprite(boardTexture);
    boardSprite.setScale(600 / 26, 600 / 26);
    sf::Sprite xSprite(xTexture);
    xSprite.setScale(20, 20);
    sf::Sprite oSprite(oTexture);
    oSprite.setScale(20, 20);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("fonts/Pixel.ttf")) {
        return -1;
    }

    // Create text and background rectangle
    sf::Text messageText;
    messageText.setFont(font);
    messageText.setCharacterSize(24);
    messageText.setFillColor(sf::Color::White);
    messageText.setPosition(150, 270);

    // Player X victories tracker
    sf::Text winsXText;
    winsXText.setFont(font);
    winsXText.setCharacterSize(24);
    winsXText.setFillColor(sf::Color::Red);
    winsXText.setPosition(150, 300);

    // Player O victories tracker
    sf::Text winsOText;
    winsOText.setFont(font);
    winsOText.setCharacterSize(24);
    winsOText.setFillColor(sf::Color::Blue);
    winsOText.setPosition(300, 300);

    sf::RectangleShape messageBackground(sf::Vector2f(400, 100));
    messageBackground.setFillColor(sf::Color::Black);
    messageBackground.setPosition(100, 250);

    // Load sound buffers
    sf::SoundBuffer placeXBuffer, placeOBuffer, victoryBuffer, drawBuffer;
    if (!placeXBuffer.loadFromFile("sounds/place_x.wav") ||
        !placeOBuffer.loadFromFile("sounds/place_o.wav") ||
        !victoryBuffer.loadFromFile("sounds/victory.wav") ||
        !drawBuffer.loadFromFile("sounds/draw.wav")) {
        return -1;
    }

    // Create sound objects
    sf::Sound placeXSound(placeXBuffer);
    sf::Sound placeOSound(placeOBuffer);
    sf::Sound victorySound(victoryBuffer);
    sf::Sound drawSound(drawBuffer);

    bool isRunning = true;
    char currentPlayer = 'X';

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mouse button pressed
            if (event.type == sf::Event::MouseButtonPressed) {
                if (isRunning) {
                    int row = event.mouseButton.y / (window.getSize().y / 3);
                    int col = event.mouseButton.x / (window.getSize().x / 3);

                    // Is a valid move
                    if (row < 3 && col < 3 && board[row][col] == ' ') {
                        playerMove(row, col, currentPlayer);
                        if (currentPlayer == 'X') {
                            placeXSound.play();
                        }
                        else {
                            placeOSound.play();
                        }

                        if (checkWin(currentPlayer)) {
                            // VICTORY (Current player wins)
                            messageText.setString("Player " + std::string(1, currentPlayer) + " wins!");

                            // Update victory tracker
                            if (currentPlayer == 'X')
                                winsX++;
                            else
                                winsO++;
                            winsXText.setString(std::to_string(winsX));
                            winsOText.setString(std::to_string(winsO));

                            victorySound.play();
                            isRunning = false;
                        }
                        else if (checkDraw()) {
                            // DRAW (Neither player wins)
                            messageText.setString("Draw!");
                            winsXText.setString(std::to_string(winsX));
                            winsOText.setString(std::to_string(winsO));
                            drawSound.play();
                            isRunning = false;
                        }

                        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                    }
                }
                else {
                    // Game is not running, restart on mouse click
                    resetGame(board, currentPlayer, isRunning, messageText, winsXText, winsOText);
                }
            }
        }

        renderBoard(window, boardSprite, xSprite, oSprite, messageBackground, messageText, winsXText, winsOText);
    }

    return 0;
}
