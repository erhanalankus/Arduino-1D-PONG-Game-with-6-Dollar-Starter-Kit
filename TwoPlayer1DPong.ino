/* Created with Visual Studio 2015 using Visual Micro extension http://www.visualmicro.com/ */

bool willTheBallGoTowardsPlayerTwo = true;
bool isInputAllowed = true;

const int playerOne = 12;
const int goalPlayerOne = 13;
const int buttonPlayerOne = 3;

const int playerTwo = 5;
const int goalPlayerTwo = 4;
const int buttonPlayerTwo = 2;

int scoreOfPlayerOne = 0;
int scoreOfPlayerTwo = 0;

const unsigned long initialMillisecondsPerLED = 500;
const unsigned long initialDeltaMillisecondsPerLED = 50;
unsigned long millisecondsPerLED = initialMillisecondsPerLED;
unsigned long deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

int currentPosition = playerOne;        //Player one starts the game.
int previousPosition = playerOne + 1;
int deltaPosition = 0;

int buttonStatePlayerOne = 0;
int lastButtonStatePlayerOne = 0;
int buttonStatePlayerTwo = 0;
int lastButtonStatePlayerTwo = 0;

void setup() {
	pinMode(4, 1);
	pinMode(5, 1);
	pinMode(6, 1);
	pinMode(7, 1);
	pinMode(8, 1);
	pinMode(9, 1);
	pinMode(10, 1);
	pinMode(11, 1);
	pinMode(12, 1);
	pinMode(13, 1);
	/*Connect pins 4 to 13 to 220ohm resistors, connect the LEDs' longer legs(+) to the resistors,
	  connect the other legs(-) of the LEDs' to ground.
	  I recommend red LEDs for 4 and 13(Goals), yellow LEDs for 5 and 12(Players), green LEDs for other pins(Field).
	*/

	pinMode(2, 0);    //Pushbuttons for players. Pin 2 is for player two. Pin 3 is for player one.
	pinMode(3, 0);    //Connect pushbuttons following the instructions on this page: https://www.arduino.cc/en/Tutorial/DigitalReadSerial
}

void loop()
{
	ListenForInput();
	currentMillis = millis();
	if (currentMillis - previousMillis >= millisecondsPerLED)  //If you don't understand this, see: https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay
	{
		CheckGoalConditions();
		DetermineNextPosition();
		MoveBallToNextPosition();

		previousMillis = currentMillis;
	}
}

void ListenForInput()    //If you don't understand this method. See: https://www.arduino.cc/en/Tutorial/StateChangeDetection
{
	buttonStatePlayerOne = digitalRead(buttonPlayerOne);
	buttonStatePlayerTwo = digitalRead(buttonPlayerTwo);

	if (buttonStatePlayerOne != lastButtonStatePlayerOne && isInputAllowed)
	{
		if (buttonStatePlayerOne == 1)
		{
			if (currentPosition == playerOne)
			{
				ToggleBallDirection();
				IncreaseSpeed();
			}
			else
			{
				ScoreForPlayer(2);
			}
		}
		lastButtonStatePlayerOne = buttonStatePlayerOne;
	}
	if (buttonStatePlayerTwo != lastButtonStatePlayerTwo && isInputAllowed)
	{
		if (buttonStatePlayerTwo == 1)
		{
			if (currentPosition == playerTwo)
			{
				ToggleBallDirection();
				IncreaseSpeed();
			}
			else
			{
				ScoreForPlayer(1);
			}
		}
		lastButtonStatePlayerTwo = buttonStatePlayerTwo;
	}
}

void ToggleBallDirection()
{
	willTheBallGoTowardsPlayerTwo = !willTheBallGoTowardsPlayerTwo;
	isInputAllowed = false;   //Only one direction change per frame is allowed for consistent gameplay.
}

void IncreaseSpeed()
{
	millisecondsPerLED -= deltaMillisecondsPerLED;
	if (deltaMillisecondsPerLED > 5)  //Because of this, it takes a little more time to reach to an insane speed. Adjust or remove this if rounds become too long.
	{
		deltaMillisecondsPerLED -= 5; 
	}
}

void MoveBallToNextPosition()      //Moves the ball one spot.
{
	previousPosition = currentPosition;
	digitalWrite(previousPosition, 0);
	currentPosition = currentPosition + deltaPosition;
	digitalWrite(currentPosition, 1);
	isInputAllowed = true;
}

void DetermineNextPosition()
{
	if (willTheBallGoTowardsPlayerTwo)
	{
		deltaPosition = -1;
	}
	else
	{
		deltaPosition = 1;
	}
}

void CheckGoalConditions()
{
	if (currentPosition == goalPlayerTwo)
	{
		ScoreForPlayer(1);
	}
	else if (currentPosition == goalPlayerOne)
	{
		ScoreForPlayer(2);
	}
}

void ScoreForPlayer(int playerWhoScored)
{
	isInputAllowed = false;
	FlashAllLEDs(1, 0);
	if (playerWhoScored == 1)
	{
		scoreOfPlayerOne++;
		ShowScores(1);
	}
	else if (playerWhoScored == 2)
	{
		scoreOfPlayerTwo++;
		ShowScores(2);
	}
	CheckEndGame();
}

void CheckEndGame()
{
	if (scoreOfPlayerOne == 3)
	{
		EndGameCeremonyFor(1);
	}
	if (scoreOfPlayerTwo == 3)
	{
		EndGameCeremonyFor(2);
	}
}

void ShowScores(int playerCurrentlyScored)
{
	if (playerCurrentlyScored == 1)
	{
		digitalWrite(playerOne, 1);
		currentPosition = playerOne;
		willTheBallGoTowardsPlayerTwo = true;
	}
	else if (playerCurrentlyScored == 2)
	{
		digitalWrite(playerTwo, 1);
		currentPosition = playerTwo;
		willTheBallGoTowardsPlayerTwo = false;
	}

	for (int i = 0; i < scoreOfPlayerOne; i++) //We use the six LEDs in the middle to show score. Each player has three green LEDs to show score. This is why three goals win the game :)
	{
		digitalWrite((11 - i), 1);
	}
	for (int i = 0; i < scoreOfPlayerTwo; i++)
	{
		digitalWrite((6 + i), 1);
	}

	delay(3000);                //Are three seconds enough for players to process the score?	
	ResetValuesForNextRound();
}

void ResetValuesForNextRound() 
{
	FlashAllLEDs(1, 0);
	millisecondsPerLED = initialMillisecondsPerLED;            //Sets speed to initial value at the beginning of each round.
	deltaMillisecondsPerLED = initialDeltaMillisecondsPerLED;  //Sets delta speed to initial value at the beginning of each round.
}

void EndGameCeremonyFor(int winner)
{
	FlashAllLEDs(50, winner);
	TurnOffAllLEDsForPlayer(0);
	scoreOfPlayerOne = 0;
	scoreOfPlayerTwo = 0;
}

void TurnOnAllLEDsForPlayer(int player)
{
	if (player != 1)          //When this method is called with (2), only these pins(player two's) will turn on
	{
		digitalWrite(4, 1);
		digitalWrite(5, 1);
		digitalWrite(6, 1);
		digitalWrite(7, 1);
		digitalWrite(8, 1);
	}
	if (player != 2)          //When this method is called with (1), only these pins(player one's) will turn on
	{
		digitalWrite(9, 1);
		digitalWrite(10, 1);
		digitalWrite(11, 1);
		digitalWrite(12, 1);
		digitalWrite(13, 1);
	}
}

void TurnOffAllLEDsForPlayer(int player)
{
	if (player != 1)          //When this method is called with (2), only these pins(player two's) will turn off
	{
		digitalWrite(4, 0);
		digitalWrite(5, 0);
		digitalWrite(6, 0);
		digitalWrite(7, 0);
		digitalWrite(8, 0);
	}
	if (player != 2)          //When this method is called with (1), only these pins(player one's) will turn off
	{
		digitalWrite(9, 0);
		digitalWrite(10, 0);
		digitalWrite(11, 0);
		digitalWrite(12, 0);
		digitalWrite(13, 0);
	}
}

void FlashAllLEDs(int blinkCount, int player) //Second parameter(int player) is for when you want to flash only one player's LEDs. Use 1 for player one, use 2 for player two. 
{
	for (int i = 0; i < blinkCount; i++)
	{
		TurnOnAllLEDsForPlayer(player);
		delay(35);
		TurnOffAllLEDsForPlayer(player);
		delay(35);
	}
}

