# Lab 3 Sorting Hat Discussion Questions

## Are all 10 questions important?

No. Not all questions are equally useful for accurate sorting. To improve user experience, we’d remove:

- **“Dawn or dusk?”** – Too abstract, weak correlation with personality.
- **“Which pet would you bring?”** – More about animal preference than traits.
- **“Favorite magical subject?”** – Fictional and often chosen arbitrarily.

Removing these improves clarity and reduces user fatigue.

---

## How to improve the model's accuracy or efficiency?

- **Prune the decision tree** to reduce overfitting.
- **Assign weights** to questions based on their impact on house prediction.
- **Use feature selection** to retain only high-value inputs.

---

## What additional sensors or hardware could enhance the user experience?

- **Heart rate sensor** – Measures excitement or stress.
- **Facial expression detection** – Adds emotional data to responses.
- **Voice sentiment analysis** – Captures tone and hesitation.

These make the sorting experience more immersive and data-rich.

---

## Is a decision tree still suitable?

Not if sensors are added. Decision trees don’t handle high-dimensional biometric data well.

- **Recommended model: Random Forest** – Handles mixed data better, more robust.
- **Alternative: SVM** – Good for complex patterns in biometric features.
