#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "sorting_hat_model.h"  

// Display settings
#define OLED_W 128
#define OLED_H 64
#define OLED_RST -1
Adafruit_SSD1306 screen(OLED_W, OLED_H, &Wire, OLED_RST);

// Button wiring (ESP32)
#define KEY_1 32
#define KEY_2 33
#define KEY_3 25
#define KEY_4 26

//  quiz prompts
const char* prompts[] = {
    "1. What guides you most?",
    "2. Rule broken — reaction?",
    "3. Fav topic at school?",
    "4. Approach a tough task?",
    "5. People describe you as?",
    "6. You find a strange book?",
    "7. Ideal magical creature?",
    "8. Strategy under pressure?",
    "9. Which friends click best?",
    "10. Life path you’d pick?"
};

//  answer phrasing
const char* choices[][4] = {
    {"A) Courage", "B) Dedication", "C) Wit", "D) Drive"},
    {"A) Speak up", "B) Forgive", "C) Notify prof", "D) Twist advantage"},
    {"A) DADA", "B) Magical plants", "C) Enchantments", "D) Alchemy"},
    {"A) Jump in", "B) Seek help", "C) Prep wisely", "D) Outsmart"},
    {"A) Fearless", "B) Loyal", "C) Bright", "D) Strategic"},
    {"A) Dive in", "B) Safety first", "C) Analyze", "D) Try spell"},
    {"A) Owl", "B) Frog", "C) Feline", "D) Flamebird"},
    {"A) Fast reflex", "B) Negotiate", "C) Think deeply", "D) Outsmart"},
    {"A) Wild spirits", "B) Devoted hearts", "C) Insightful minds", "D) Confident alphas"},
    {"A) Dark wizard hunter", "B) Magical doctor", "C) Arcane scholar", "D) Magical leader"}
};

// Store answers as index (1–4)
int selections[10] = {0};
int currentPrompt = 0;

// ML classifier from model
Eloquent::ML::Port::DecisionTree brain;

void setup() {
    Serial.begin(115200);

    // OLED setup
    if (!screen.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("⚠️ Display failed. Fix wiring.");
        while (true);
    }

    screen.clearDisplay();
    screen.setTextSize(1);
    screen.setTextColor(WHITE);
    screen.setCursor(5, 10);
    screen.println("🧠 Enchanted Hat Awake");
    screen.display();
    delay(1500);

    // Button setup
    pinMode(KEY_1, INPUT_PULLUP);
    pinMode(KEY_2, INPUT_PULLUP);
    pinMode(KEY_3, INPUT_PULLUP);
    pinMode(KEY_4, INPUT_PULLUP);

    presentPrompt();
}

void loop() {
    checkForInput();
}

// Show question + options
void presentPrompt() {
    screen.clearDisplay();
    screen.setTextSize(1);
    screen.setCursor(0, 0);
    screen.println(prompts[currentPrompt]);

    for (int i = 0; i < 4; i++) {
        screen.setCursor(10, 20 + (i * 10));
        screen.println(choices[currentPrompt][i]);
    }

    screen.display();
}

// Check buttons and save answer
void checkForInput() {
    bool picked = false;

    while (!picked) {
        if (digitalRead(KEY_1) == LOW) {
            selections[currentPrompt] = 1;
            picked = true;
        } else if (digitalRead(KEY_2) == LOW) {
            selections[currentPrompt] = 2;
            picked = true;
        } else if (digitalRead(KEY_3) == LOW) {
            selections[currentPrompt] = 3;
            picked = true;
        } else if (digitalRead(KEY_4) == LOW) {
            selections[currentPrompt] = 4;
            picked = true;
        }
        delay(20);
    }

    while (digitalRead(KEY_1) == LOW || digitalRead(KEY_2) == LOW ||
           digitalRead(KEY_3) == LOW || digitalRead(KEY_4) == LOW) {
        delay(20);
    }

    nextPrompt();
}

// Go to next question or classify
void nextPrompt() {
    currentPrompt++;
    if (currentPrompt < 10) {
        presentPrompt();
    } else {
        determineAffinity();
    }
}

// Predict the House
void determineAffinity() {
    screen.clearDisplay();
    screen.setTextSize(1);
    screen.setCursor(15, 20);
    screen.println("🔮 Calculating...");
    screen.display();
    delay(1000);

    float input[10];
    for (int i = 0; i < 10; i++) input[i] = (float)selections[i];

    int outcome = brain.predict(input);

    screen.clearDisplay();
    screen.setTextSize(2);
    screen.setCursor(0, 20);
    screen.print("Result:\n");

    switch (outcome) {
        case 0: screen.println("Gryffindor"); break;
        case 1: screen.println("Hufflepuff"); break;
        case 2: screen.println("Ravenclaw"); break;
        case 3: screen.println("Slytherin"); break;
        default: screen.println("Unknown"); break;
    }

    screen.display();

    Serial.println("✨ Sorting Done");
    Serial.print("Assigned House: ");
    switch (outcome) {
        case 0: Serial.println("Gryffindor"); break;
        case 1: Serial.println("Hufflepuff"); break;
        case 2: Serial.println("Ravenclaw"); break;
        case 3: Serial.println("Slytherin"); break;
    }
}
