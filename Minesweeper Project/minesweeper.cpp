#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <queue>
#include <chrono>
#include <random>
#include <set>

using namespace std;

enum class TileState {
    Hidden,
    Flagged,
    Shown
};

enum class GameState {
    Running,
    Paused,
    Won,
    Lost,
};

/*struct BoardState {
    int width;
    int height;
    int mineCount;
    int tileCount;
};*/


void windowDimen(int& width, int& height, int& mineCount, int& tileCount) {
    // Open the board config file
    ifstream configFile("board_config.cfg");

    // Throw an exception if the file cannot be opened
    if (!configFile) {
        throw runtime_error("Failed to open board config file.");
    }

    int numColumns, numRows;

    // Read the number of columns, rows, mine count, and tile count from the config file
    configFile >> numColumns >> numRows >> mineCount >> tileCount;

    // Calculate the width and height of the game window based on the board configuration
    width = numColumns * 32;
    height = numRows * 32 + 100;
}

void displayboard(const vector<vector<int>> &v) {
    for (const auto& row : v) {
        for (const auto& element : row) {
            cout << setw(4) << element << " ";
        }
        cout << '\n';
    }
}

void centerText(sf::Text& text, float centerX, float centerY) {
    // Get the local bounds of the text object
    sf::FloatRect textRect = text.getLocalBounds();

    // Calculate the origin so that the text is centered
    float originX = textRect.left + textRect.width / 2.0f;
    float originY = textRect.top + textRect.height / 2.0f;

    // Set the origin and position of the text object
    text.setOrigin(originX, originY);
    text.setPosition(centerX, centerY);
}





void initializeGameBoard(vector<vector<int>> &gameBoard, vector<vector<TileState>> &tileStates, const int &numRows, const int &numCols, const int &mineCount) {
// Initialize the game board with random mine placement
// Use a random_device to generate a seed for the random number generator
    random_device randomDevice;
    mt19937 randomNumberGenerator(randomDevice());
// Create a set to store the coordinates of the mines
    set<pair<int, int>> mineCoordinates;
    while (mineCoordinates.size() < mineCount) {
        int row = uniform_int_distribution<int>(0, numRows - 1)(randomNumberGenerator);
        int col = uniform_int_distribution<int>(0, numCols - 1)(randomNumberGenerator);
        mineCoordinates.insert({row, col});
    }

// Initialize the game board and tile states
    gameBoard.assign(numRows, vector<int>(numCols, 0));
    tileStates.assign(numRows, vector<TileState>(numCols, TileState::Hidden));
    for (auto [row, col] : mineCoordinates) {
        gameBoard[row][col] = -1;
        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if (r >= 0 && r < numRows && c >= 0 && c < numCols && gameBoard[r][c] != -1) {
                    gameBoard[r][c]++;
                }
            }
        }
    }
}


void revealEmptyTiles(vector<vector<int>> &board, vector<vector<TileState>> &tileStates, const int &row, const int &col, int &tilesRevealed) {
    queue<pair<int, int>> tilesToVisit;
    set<pair<int, int>> visitedTiles;
    tilesToVisit.emplace(row, col);    // Pushing the current tile
    visitedTiles.insert({row, col});
    while (!tilesToVisit.empty()) {
        auto currentTile = tilesToVisit.front();
        tilesToVisit.pop();
        int currentRow = currentTile.first, currentCol = currentTile.second;
        if (tileStates[currentRow][currentCol] == TileState::Shown || board[currentRow][currentCol] == -1) {
            continue;
        }
        if (tileStates[currentRow][currentCol] != TileState::Flagged) {
            tileStates[currentRow][currentCol] = TileState::Shown;
            tilesRevealed++;
        }
        if (board[currentRow][currentCol] == 0) {
            for (int rowOffset = -1; rowOffset <= 1; rowOffset++) {
                for (int colOffset = -1; colOffset <= 1; colOffset++) {
                    int neighborRow = currentRow + rowOffset, neighborCol = currentCol + colOffset;
                    if (neighborRow < 0 || neighborRow >= board.size() || neighborCol < 0 || neighborCol >= board[0].size()) {
                        continue;
                    }
                    if (visitedTiles.count({neighborRow, neighborCol}) == 0) {
                        tilesToVisit.emplace(neighborRow, neighborCol);      // Pushing the neighbors of the current tile
                        visitedTiles.insert({neighborRow, neighborCol});
                    }
                }
            }
        }
    }
}

void displayLeaderboard(const int& windowWidth, const int& windowHeight, const sf::Font& font, const string& playerName) {
// Create the SFML window
    sf::RenderWindow leaderboardWindow(sf::VideoMode(windowWidth / 2, windowHeight / 2), "Leaderboard", sf::Style::Titlebar | sf::Style::Close);
    leaderboardWindow.setFramerateLimit(60);

// Set up the leaderboard title text
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(20);
    titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    titleText.setString("LEADERBOARD");
    titleText.setFillColor(sf::Color::White);
    centerText(titleText, leaderboardWindow.getSize().x / 2.0f, leaderboardWindow.getSize().y / 2.0f - 120);

// Set up the leaderboard text
    sf::Text leaderboardText;
    leaderboardText.setFont(font);
    leaderboardText.setCharacterSize(18);
    leaderboardText.setStyle(sf::Text::Bold);
    leaderboardText.setFillColor(sf::Color::White);

// Open the leaderboard file and read its contents
    ifstream leaderboardFile("leaderboard.txt");
    if (!leaderboardFile.is_open()) {
        cerr << "Error: Unable to open leaderboard file." << endl;
        return;
    }

// Loop through the first five non-empty lines of the leaderboard file
    string row;
    string leaderboardContent;
    for (char index = '1'; index <= '5' && getline(leaderboardFile, row); index++) {
        // Check if the player's name is already in the leaderboard
        if (row.substr(6) == playerName) {
            // Add an asterisk to indicate the player's score
            row += "*";
        }
        // Format the row with a rank and add it to the leaderboard content
        string formattedRow = ".\t" + row.substr(0, 5) + "\t" + row.substr(6) + "\n\n";
        leaderboardContent += index + formattedRow;
    }

    leaderboardFile.close();

// Set the leaderboard text content and position
    leaderboardText.setString(leaderboardContent);
    centerText(leaderboardText, leaderboardWindow.getSize().x / 2.0f, leaderboardWindow.getSize().y / 2.0f + 20);

    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                leaderboardWindow.close();
            }
        }

        leaderboardWindow.clear(sf::Color::Blue);

        // Draw the leaderboard title and text
        leaderboardWindow.draw(titleText);
        leaderboardWindow.draw(leaderboardText);

        // Display the window
        leaderboardWindow.display();
    }
}



void addScoreToLeaderboard(const int& timeInSeconds, const string& playerName, const bool& hasBeenCalledBefore) {
    // Check if the function has already been called before
    if (hasBeenCalledBefore) {
        cerr << "Error: This function has already been called before" << endl;
        return;
    }

    // Convert the time to minutes and seconds
    int minutes = timeInSeconds / 60;
    int seconds = timeInSeconds % 60;

    // Format the time as a string
    ostringstream timeStringStream;
    timeStringStream << setfill('0') << setw(2) << minutes << ":" << setw(2) << seconds;
    string timeString = timeStringStream.str();

    // Read the leaderboard file into a vector of strings
    ifstream leaderboardFileIn("leaderboard.txt");
    if (!leaderboardFileIn.is_open()) {
        cerr << "Error: Unable to open leaderboard file" << endl;
        return;
    }
    vector<string> leaderboardEntries;
    string line;
    while (getline(leaderboardFileIn, line)) {
        leaderboardEntries.push_back(line);
    }
    leaderboardFileIn.close();

    // Insert the new score into the appropriate position in the vector
    bool inserted = false;
    for (auto it = leaderboardEntries.begin(); it != leaderboardEntries.end(); ++it) {
        string currentEntryTimeString = it->substr(0, 5);
        int currentEntryMinutes = stoi(currentEntryTimeString.substr(0, 2));
        int currentEntrySeconds = stoi(currentEntryTimeString.substr(3, 2));
        if (minutes < currentEntryMinutes || (minutes == currentEntryMinutes && seconds < currentEntrySeconds)) {
            leaderboardEntries.insert(it, timeString + "," + playerName);
            inserted = true;
            break;
        }
    }
    if (!inserted) {
        leaderboardEntries.push_back(timeString + "," + playerName);
    }

    // Write the updated leaderboard back to the file
    ofstream leaderboardFileOut("leaderboard.txt");
    if (!leaderboardFileOut.is_open()) {
        cerr << "Error: Unable to open leaderboard file for writing" << endl;
        return;
    }
    for (const auto& entry : leaderboardEntries) {
        leaderboardFileOut << entry << endl;
    }
    leaderboardFileOut.close();
}

void checkAndDisplayLeaderboard(sf::RenderWindow& gameWindow, bool& closed, GameState gameState, int width, int height, sf::Font& font, std::string name) {
    if (!closed && gameState == GameState::Won) {
        displayLeaderboard(width, height, font, name);
        closed = true;
    }
}



int main() {
    // Initialize variables
    int windowWidth = 0, windowHeight = 0, numMines = 0, numTiles= 0;

    // Call function to get window dimensions and mine/tile counts
    windowDimen(windowWidth, windowHeight, numMines, numTiles);

    // Set mine count to constant variable for easy access
    const int unchangedNumMines = numMines;

    // Create SFML window with specified dimensions and title
    sf::RenderWindow welcomeWindow(sf::VideoMode(windowWidth, windowHeight), "Welcome Window", sf::Style::Titlebar | sf::Style::Close);

    // Set window framerate limit
    welcomeWindow.setFramerateLimit(60);

    // Load font file for text
    sf::Font Font;
    if (!Font.loadFromFile("font.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return 1;
    }

    // Create welcome text
    sf::Text welcomeText("WELCOME TO MINESWEEPER!", Font, 24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    welcomeText.setFillColor(sf::Color::White);

    // Position welcome text at center of screen
    centerText(welcomeText, (float) welcomeWindow.getSize().x / 2.0f, (float) welcomeWindow.getSize().y / 2.0f - 150);

    // Create input prompt text
    sf::Text userText("Enter your name:", Font, 20);
    userText.setStyle(sf::Text::Bold);
    userText.setFillColor(sf::Color::White);

    // Position input prompt text at center of screen
    centerText(userText, (float) welcomeWindow.getSize().x / 2.0f, (float) welcomeWindow.getSize().y / 2.0f - 75);

    // Create input text
    sf::Text inputText("", Font, 18);
    inputText.setStyle(sf::Text::Bold);
    inputText.setFillColor(sf::Color::Yellow);

    // Position input text at center of screen
    centerText(inputText, (float) welcomeWindow.getSize().x / 2.0f, (float) welcomeWindow.getSize().y / 2.0f - 45);

    // Create cursor background rectangle for input text
    sf::RectangleShape cursorSquare(sf::Vector2f(2.0f, inputText.getGlobalBounds().height));
    cursorSquare.setFillColor(sf::Color::Transparent);

    // Set default name to "|"
    std::string name = "|";

    // Set flag to keep track of whether name is valid
    bool validNameEntered = false;

    // Event loop for the welcome window
    while (welcomeWindow.isOpen()) {
        sf::Event event{};
        while (welcomeWindow.pollEvent(event)) {
// Handle window close event
            if (event.type == sf::Event::Closed) {
                welcomeWindow.close();
                return 0;
            }
            // Handle text input event
            inputText.setString(name);
            if (event.type == sf::Event::TextEntered) {
                // Add new character to name if it is a letter and name is less than 10 characters long
                if ((event.text.unicode >= 'A' && event.text.unicode <= 'Z') ||
                    (event.text.unicode >= 'a' && event.text.unicode <= 'z') && name.size() < 10) { // limit name to 10 characters
                    name.pop_back(); // remove cursor character
                    name += static_cast<char>(event.text.unicode);
                    name.push_back('|'); // add cursor character
                    inputText.setString(name);
                    centerText(inputText, (float) welcomeWindow.getSize().x / 2.0f,
                               (float) welcomeWindow.getSize().y / 2.0f - 35);

                    // update cursor background size to fit new input text
                    cursorSquare.setSize(sf::Vector2f(inputText.getGlobalBounds().width + 10.0f,
                                                      inputText.getGlobalBounds().height + 10.0f));
                }
                    // Handle backspace event
                else if (event.text.unicode == '\b') {
                    if (!name.empty()) {
                        name.pop_back(); // remove cursor character
                        name.pop_back(); // remove last character
                        inputText.setString(name);
                        name.push_back('|'); // add cursor character
                        centerText(inputText, (float) welcomeWindow.getSize().x / 2.0f,
                                   (float) welcomeWindow.getSize().y / 2.0f - 35);

                        // update cursor background size to fit new input text
                        cursorSquare.setSize(sf::Vector2f(inputText.getGlobalBounds().width + 10.0f,
                                                          inputText.getGlobalBounds().height + 10.0f));
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (name.size() > 1 && event.key.code == sf::Keyboard::Enter) {
                    name.pop_back();
                    welcomeWindow.close();
                }
            }
        }
        welcomeWindow.clear(sf::Color::Blue); // set the background color to blue
        welcomeWindow.draw(welcomeText);
        welcomeWindow.draw(userText);

        // draw cursor background first so it's behind the input text
        cursorSquare.setPosition(inputText.getPosition().x - 5.0f, inputText.getPosition().y - 5.0f);
        welcomeWindow.draw(cursorSquare);

        welcomeWindow.draw(inputText);
        welcomeWindow.display();
    }

// Convert name to lowercase
    std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });

// Capitalize first letter
    if (!name.empty()) {
        name[0] = (char) std::toupper(name[0]);
    }


// -----------------------------------GAME WINDOW -----------------------------------------
// Set up grid size
    const int tilesHeight = 32;
    const int numRows = (windowHeight - 100) / tilesHeight;
    const int numCols = windowWidth / tilesHeight;

// Create the game window
    sf::RenderWindow gameWindow;
    gameWindow.create(sf::VideoMode(windowWidth, windowHeight), "Minesweeper", sf::Style::Titlebar | sf::Style::Close);
    gameWindow.setFramerateLimit(60);

// Initialize game board and tile states
    std::vector<std::vector<int>> tileboard;
    tileboard.resize(numRows, std::vector<int>(numCols, 0));
    std::vector<std::vector<TileState>> tileState;
    tileState.resize(numRows, std::vector<TileState>(numCols, TileState::Hidden));
    initializeGameBoard(tileboard, tileState, numRows, numCols, unchangedNumMines);

// Set up game state
    GameState isplaying = GameState::Running;
    bool addedNewScore(false);
    int tilesShown(0);
    bool debugged(false);
    auto paused = std::chrono::high_resolution_clock::now();

// Set up leaderboard window
    bool closed(false);
    bool showAgain(false);
    bool showLeaderBoard(false);
    displayboard(tileboard);

// Load number textures
    std::vector<sf::Texture> numberTextures;
    for (int i = 1; i <= 8; i++) {
        std::stringstream ss;
        ss << "number_" << i << ".png";
        sf::Texture texture;
        if (!texture.loadFromFile(ss.str())) {
            std::cerr << "Failed to load number texture " << i << "!" << std::endl;
            return 1;
        }
        numberTextures.push_back(texture);
    }

    sf::Texture textures[12];
    std::string filenames[12] = {"mine.png", "tile_hidden.png", "tile_revealed.png", "face_happy.png",
                                 "face_win.png", "face_lose.png", "digits.png", "debug.png", "pause.png",
                                 "play.png", "flag.png", "leaderboard.png"};

    for (int i = 0; i < 12; i++) {
        if (!textures[i].loadFromFile(filenames[i])) {
            std::cerr << "Failed to load texture: " << filenames[i] << std::endl;
            return 1;
        }
    }

// Set up sprites
    sf::Sprite mineSprite(textures[0]), hiddenSprite(textures[1]), revealedSprite(textures[2]),
            happyFaceSprite(textures[3]), winFaceSprite(textures[4]), loseFaceSprite(textures[5]),
            digitsSprite(textures[6]), debugSprite(textures[7]), pauseSprite(textures[8]),
            playSprite(textures[9]), flagSprite(textures[10]), leaderboardSprite(textures[11]),
            timerSprite(textures[6]);

    std::vector<sf::Sprite> numberSprites;
    for (int i = 0; i < 8; i++) {
        numberSprites.push_back(sf::Sprite(numberTextures[i]));
    }


// Start the timer
    auto start_time = chrono::high_resolution_clock::now();
    auto elapsed_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time).count();

    //Main looper
    while (gameWindow.isOpen()) {
        sf::Event event{};
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            // Handle left click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && isplaying == GameState::Running) {
                int row = event.mouseButton.y / 32; // Calculate row based on mouse y-coordinate
                int col = event.mouseButton.x / 32; // Calculate column based on mouse x-coordinate
                if (row >= 0 && row < numRows && col >= 0 && col < numCols) { // Ensure the clicked position is within the game board
                    if (tileState[row][col] == TileState::Hidden) {
                        if (tileboard[row][col] == -1) { // Bomb tile
                            // Reveal all bomb tiles
                            for (int i = 0; i < numRows; i++) {
                                for (int j = 0; j < numCols; j++) {
                                    if (tileboard[i][j] == -1) {
                                        tileState[i][j] = TileState::Shown;
                                        tilesShown++;
                                    }
                                }
                            }
                            isplaying = GameState::Lost;
                        } else if (tileboard[row][col] == 0) { // Empty tile
                            //Reveal all adjacent empty tiles
                            revealEmptyTiles(tileboard, tileState, row, col, tilesShown);
                        } else { // Number tile
                            tileState[row][col] = TileState::Shown;
                            tilesShown++;
                        }
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && isplaying == GameState::Running) {
                int row = event.mouseButton.y / 32; // Calculate row based on mouse y-coordinate
                int col = event.mouseButton.x / 32; // Calculate column based on mouse x-coordinate
                TileState& tile = tileState[row][col]; // Reference to the tile state
                switch (tile) {
                    case TileState::Hidden:
                        tile = TileState::Flagged;
                        numMines--;
                        break;
                    case TileState::Flagged:
                        tile = TileState::Hidden;
                        numMines++;
                        break;
                    default:
                        break;
                }
            }
        }
        // Check if the player has won
        if (tilesShown == (numRows * numCols) - numMines) {
            isplaying = GameState::Won;
            bool allMinesFlagged = true;
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    if (tileboard[i][j] == -1) {
                        if (tileState[i][j] != TileState::Flagged) {
                            allMinesFlagged = false;
                            tileState[i][j] = TileState::Shown;
                            sf::Sprite s = hiddenSprite;
                            s.setPosition((float) j * 32.0f, (float) i * 32.0f);
                            gameWindow.draw(s);
                        }
                    } else {
                        tileState[i][j] = TileState::Shown;
                    }
                }
            }
            if (allMinesFlagged) {
                elapsed_time = chrono::duration_cast<chrono::seconds>(
                        chrono::high_resolution_clock::now() - start_time).count();
                int time_elapsed = (int) elapsed_time;
                addScoreToLeaderboard(time_elapsed, name, addedNewScore);
                addedNewScore = true;
            }
        }

        // Set the background color of the game window to white
        gameWindow.clear(sf::Color::White);
        // Draw the tiles
        for (auto row = tileState.begin(); row != tileState.end(); row++) {
            for (auto col = row->begin(); col != row->end(); col++) {
                int i = distance(tileState.begin(), row);
                int j = distance(row->begin(), col);

                sf::Sprite sprite = hiddenSprite;
                if (isplaying == GameState::Paused) {
                    sprite = revealedSprite;
                    sprite.setPosition((float)j * 32.0f, (float)i * 32.0f);
                    gameWindow.draw(sprite);
                    continue;
                }

                if (*col == TileState::Shown) {
                    sprite = revealedSprite;
                    sprite.setPosition((float)j * 32.0f, (float)i * 32.0f);
                    gameWindow.draw(sprite);

                    if (tileboard[i][j] > 0) {
                        sprite = numberSprites[tileboard[i][j] - 1];
                    }
                    else if (tileboard[i][j] == -1) {
                        sprite = mineSprite;
                    }
                }
                else {
                    sprite = hiddenSprite;
                    if (debugged) {
                        sprite.setPosition((float)j * 32.0f, (float)i * 32.0f);
                        gameWindow.draw(sprite);

                        if (tileboard[i][j] == -1) {
                            sprite = mineSprite;
                        }
                    }
                }

                sprite.setPosition((float)j * 32.0f, (float)i * 32.0f);
                gameWindow.draw(sprite);

                if (*col == TileState::Flagged) {
                    flagSprite.setPosition((float)j * 32.0f, (float)i * 32.0f);
                    gameWindow.draw(flagSprite);
                }
            }
        }

        if (showAgain) {
            // Open the leaderboard window
            displayLeaderboard(windowWidth, windowHeight, Font, name);
            start_time += chrono::high_resolution_clock::now() - paused;
            isplaying = GameState::Running;
            showAgain = false;
        }

        // Draw the face button
        sf::Sprite faceSprite;
        if (isplaying == GameState::Won) {
            faceSprite = winFaceSprite;
        } else if (isplaying == GameState::Lost) {
            faceSprite = loseFaceSprite;
        } else {
            faceSprite = happyFaceSprite;
        }
        faceSprite.setPosition(((float) numCols / 2.0f * 32.0f) - 32.0f, 32.0f * ((float) numRows + 0.5f));
        gameWindow.draw(faceSprite);

// Draw the debug button
        debugSprite.setPosition((float) numCols * 32.0f - 304.0f, 32.0f * ((float) numRows + 0.5f));
        gameWindow.draw(debugSprite);

// Draw the pause/play button
        sf::Sprite pausePlaySprite = (isplaying == GameState::Paused) ? pauseSprite : playSprite;
        pausePlaySprite.setPosition((float) numCols * 32.0f - 240.0f, 32.0f * ((float) numRows + 0.5f));
        gameWindow.draw(pausePlaySprite);

        // Draw the leaderboard button
        leaderboardSprite.setPosition((float) numCols * 32.0f - 176.0f, 32.0f * ((float) numRows + 0.5f));
        gameWindow.draw(leaderboardSprite);

        bool isLeftMouseButtonPressed = event.type == sf::Event::MouseButtonPressed &&
                                        event.mouseButton.button == sf::Mouse::Left;

        if (isLeftMouseButtonPressed) {
            bool isFaceSpriteClicked = faceSprite.getGlobalBounds().contains((float) event.mouseButton.x, (float) event.mouseButton.y);
            bool isDebugSpriteClicked = debugSprite.getGlobalBounds().contains((float) event.mouseButton.x, (float) event.mouseButton.y);
            bool isPausePlaySpriteClicked = pausePlaySprite.getGlobalBounds().contains((float) event.mouseButton.x, (float) event.mouseButton.y);
            bool isLeaderboardSpriteClicked = leaderboardSprite.getGlobalBounds().contains((float) event.mouseButton.x, (float) event.mouseButton.y);

            if (isFaceSpriteClicked) {
                // Restart the game
                initializeGameBoard(tileboard, tileState, numRows, numCols, unchangedNumMines);
                tilesShown = 0;
                addedNewScore = false;
                debugged = false;
                closed = false;
                isplaying = GameState::Running;
                numMines = unchangedNumMines;
                start_time = chrono::high_resolution_clock::now();
            } else if (isDebugSpriteClicked) {
                // Toggle debug mode
                debugged = !debugged;
            } else if (isPausePlaySpriteClicked) {
                if (isplaying == GameState::Paused) {
                    // Resume game
                    isplaying = GameState::Running;
                    start_time += chrono::high_resolution_clock::now() - paused;
                } else if (isplaying == GameState::Running) {
                    // Pause game
                    isplaying = GameState::Paused;
                    paused = chrono::high_resolution_clock::now();
                }
            } else if (isLeaderboardSpriteClicked) {
                // Open leaderboard window
                isplaying = GameState::Paused;
                paused = chrono::high_resolution_clock::now();
                showAgain = true;
            }
        }
// ended off ---------------------------------------------
// Draw the mine counter
        sf::Vector2f counterPos(33.0f, 32 * ((float) numRows + 0.5f) + 16);
        string counter = "000";
        int tCount = abs(numMines);

// Extract the individual digits from tCount and update the counter string
        counter = to_string(tCount);
        if (numMines < 0) {
            // Draw the negative sign sprite
            sf::Sprite negativeSprite(textures[6]);
            negativeSprite.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
            negativeSprite.setPosition(counterPos.x, counterPos.y);
            gameWindow.draw(negativeSprite);
            // Update the position for the next digit sprite
            counterPos.x += 21;
        }

// Pad the counter string with leading zeros if necessary
        if (counter.length() < 3) {
            counter = string(3 - counter.length(), '0') + counter;
        }

// Draw each digit of the mine counter
        for (int i = 0; i < counter.length(); i++) {
            int digit = counter[i] - '0';
            sf::Sprite digitSprite(textures[6]);
            digitSprite.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
            digitSprite.setPosition(counterPos.x, counterPos.y);
            gameWindow.draw(digitSprite);
            // Update the position for the next digit sprite
            counterPos.x += 21;
        }

        // Calculate elapsed time
        if (isplaying == GameState::Running) {
            elapsed_time = chrono::duration_cast<chrono::seconds>(
                    chrono::high_resolution_clock::now() - start_time).count();
        }
        int timeElapsed = (int) elapsed_time;
// Draw the timer
        int minutes = timeElapsed / 60;
        int seconds = timeElapsed % 60;
        sf::Vector2f minutesPos(((float) numCols * 32.0f) - 97, 32 * ((float) numRows + 0.5f) + 16);
        sf::Vector2f secondsPos(((float) numCols * 32.0f) - 54, 32 * ((float) numRows + 0.5f) + 16);
// Draw the minutes digits
        string minutesStr = to_string(minutes);
        if (minutesStr.length() < 2) {
            minutesStr = "0" + minutesStr;
        }
        sf::Sprite digitSprite(textures[6]);
        digitSprite.setTextureRect(sf::IntRect((minutesStr[0] - '0') * 21, 0, 21, 32));
        digitSprite.setPosition(minutesPos.x, minutesPos.y);
        gameWindow.draw(digitSprite);
        digitSprite.setTextureRect(sf::IntRect((minutesStr[1] - '0') * 21, 0, 21, 32));
        digitSprite.setPosition(minutesPos.x + 21, minutesPos.y);
        gameWindow.draw(digitSprite);
// Draw the seconds digits
        string secondsStr = to_string(seconds);
        if (secondsStr.length() < 2) {
            secondsStr = "0" + secondsStr;
        }
        digitSprite.setTextureRect(sf::IntRect((secondsStr[0] - '0') * 21, 0, 21, 32));
        digitSprite.setPosition(secondsPos.x, secondsPos.y);
        gameWindow.draw(digitSprite);
        digitSprite.setTextureRect(sf::IntRect((secondsStr[1] - '0') * 21, 0, 21, 32));
        digitSprite.setPosition(secondsPos.x + 21, secondsPos.y);
        gameWindow.draw(digitSprite);

        // Display everything that has been drawn
        gameWindow.display();

        checkAndDisplayLeaderboard(gameWindow, closed, isplaying, windowWidth, windowHeight, Font, name);
    }
    return 0;
}