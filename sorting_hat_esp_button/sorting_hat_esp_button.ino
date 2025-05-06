#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sorting_hat_model.h"  // Include the trained Decision Tree model

// OLED screen configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button pins for ESP32-WROOM
#define BUTTON_A  32
#define BUTTON_B  33
#define BUTTON_C  25
#define BUTTON_D  26

// Quiz questions
const char* questions[] = {
    "1. What do you value?",
    "2. Someone cheats?",
    "3. Favorite subject?",
    "4. Face challenges?",
    "5. Friends say you're?",
    "6. Mystery book?",
    "7. Preferred pet?",
    "8. Solve problems?",
    "9. Friends you like?",
    "10. Dream career?"
};

// Answer options for each question
const char* options[][4] = {
    {"A) Bravery", "B) Loyalty", "C) Intelligence", "D) Ambition"},
    {"A) Call out", "B) Let go", "C) Tell teacher", "D) Gain from it"},
    {"A) Defense", "B) Herbology", "C) Charms", "D) Potions"},
    {"A) Head-on", "B) Team up", "C) Plan first", "D) Outsmart"},
    {"A) Bold", "B) Kind", "C) Smart", "D) Clever"},
    {"A) Read now", "B) Check safe", "C) Study it", "D) Use it"},
    {"A) Owl", "B) Toad", "C) Cat", "D) Phoenix"},
    {"A) Act fast", "B) Compromise", "C) Analyze", "D) Outsmart"},
    {"A) Adventurous", "B) Loyal", "C) Thoughtful", "D) Powerful"},
    {"A) Auror", "B) Healer", "C) Scholar", "D) Minister"}
};

// Store user responses (1-4 per question)
int responses[10] = {0};
int questionIndex = 0;

// Load ML decision tree classifier
Eloquent::ML::Port::DecisionTree clf;

void setup() {
    Serial.begin(115200);

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("⚠️ OLED init failed. Check wiring.");
        while (true);  // Halt if OLED init fails
    }

    // Show startup message
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(10, 10);
    display.println("Sorting Hat Ready!");
    display.display();
    delay(1500);

    // Initialize buttons as input with internal pull-ups
    pinMode(BUTTON_A, INPUT_PULLUP);
    pinMode(BUTTON_B, INPUT_PULLUP);
    pinMode(BUTTON_C, INPUT_PULLUP);
    pinMode(BUTTON_D, INPUT_PULLUP);

    // Display the first question
    showQuestion();
}

void loop() {
    // Continuously check for user input
    checkButtons();
}

// Display current question and options
void showQuestion() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(questions[questionIndex]);

    // Print all 4 options
    for (int i = 0; i < 4; i++) {
        display.setCursor(10, 20 + (i * 10));
        display.println(options[questionIndex][i]);
    }

    display.display();
}

// Check if a button was pressed and record response
void checkButtons() {
    bool pressed = false;

    while (!pressed) {
        if (digitalRead(BUTTON_A) == LOW) {
            responses[questionIndex] = 1;
            pressed = true;
        } else if (digitalRead(BUTTON_B) == LOW) {
            responses[questionIndex] = 2;
            pressed = true;
        } else if (digitalRead(BUTTON_C) == LOW) {
            responses[questionIndex] = 3;
            pressed = true;
        } else if (digitalRead(BUTTON_D) == LOW) {
            responses[questionIndex] = 4;
            pressed = true;
        }
        delay(20);  // Debounce delay
    }

    // Wait until button is released before proceeding
    while (digitalRead(BUTTON_A) == LOW ||
           digitalRead(BUTTON_B) == LOW ||
           digitalRead(BUTTON_C) == LOW ||
           digitalRead(BUTTON_D) == LOW) {
        delay(20);
    }

    nextQuestion();
}

// Go to next question or trigger classification
void nextQuestion() {
    questionIndex++;
    if (questionIndex < 10) {
        showQuestion();  // Display next question
    } else {
        classifyHouse();  // All responses collected, predict house
    }
}

// Use the ML model to predict the Hogwarts house
void classifyHouse() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(10, 10);
    display.println("Sorting...");
    display.display();
    delay(1000);

    // Convert responses to float array for model input
    float features[10];
    for (int i = 0; i < 10; i++) {
        features[i] = (float)responses[i];
    }

    // Predict house (0–3)
    int house = clf.predict(features);

    // Display result on screen
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.print("House:\n");
    switch (house) {
        case 0: display.println("Gryffindor"); break;
        case 1: display.println("Hufflepuff"); break;
        case 2: display.println("Ravenclaw"); break;
        case 3: display.println("Slytherin"); break;
        default: display.println("Unknown"); break;
    }

    display.display();

    // Log to Serial Monitor
    Serial.println("✅ Sorting complete!");
    Serial.print("Predicted House: ");
    switch (house) {
        case 0: Serial.println("Gryffindor"); break;
        case 1: Serial.println("Hufflepuff"); break;
        case 2: Serial.println("Ravenclaw"); break;
        case 3: Serial.println("Slytherin"); break;
    }
}
