# Smart Classroom Comfort Monitor

## Responsibilities

- Monitor classroom temperatures and identify when conditions fall outside an acceptable range.

## Requirements

- Allow user to enter temperature in fahrenheit.
- Determine whether temp is within 68-76F inclusive.
- Display NORMAL or WARNING status.
- Calculate average of the recorded temperatures
- Save temp readings and their status to a file.
- Use LED to show warning condition.
- Allow the acceptable temp range to be changed.

## Diagram/Achitecture

1. Saves user temp input in an array called readings
2. Calculates avg and print it
3. Determine each temp and saves its status in a new array called status
4. Show status with LED on/off
4. Saves all temp and status and led to a file.