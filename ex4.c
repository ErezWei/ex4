/******************
Name:
ID:
Assignment: ex4
*******************/
#include <stdio.h>
#include <string.h>

#define PYRAMID_SIZE 5
#define MAX_GRID 20
#define PREV_PLACEMENT_INIT (-2)
#define MAX_CW_GRID 30
#define MAX_SLOTS 100
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 16

struct slot {
	int row;
	int column;
	int length;
	char direction;
};

struct word {
	char value[MAX_WORD_LENGTH];
	int length;
	int wasUsed;
	int slotIndex;
};

int task1RobotPaths(int column, int row);
void task2HumanPyramid();
float task2WeightCalc(float weights[][PYRAMID_SIZE], int pyramidSize, int requestedRow, int requestedColumn);
void task3ParenthesisValidator();
int task3NewOpenParenthesis(char openParenthesis);
void task4QueensBattle();
void task4CreateAreasList(char inputGrid[][MAX_GRID], char areasList[], int size, int row, int column, int areasAdded);
int task4IsAreaInArray(char areasList[], int size, int startAt, char areaToCheck);
int task4PlacementColumns(char inputGrid[][MAX_GRID], int outputGrid[][MAX_GRID], int occupiedRows[],
	char areasList[], int size, int column, int row, int prevPlacement);
int task4PlacementRows(char inputGrid[][MAX_GRID], int occupiedRows[], char areasList[],
	int size, int row, int column, int prevPlacement);
void task4RemoveArea(char oldAreasList[], char newAreasList[], int size, int startAt, char areaToRemove);
void task5CrosswordGenerator();
int task5FillSlots(char grid[][MAX_CW_GRID], int gridSize, struct slot slots[], int numSlots,
	struct word words[], int numWords, int slotIndex, int wordIndex);
int task5CheckMatch(char grid[][MAX_CW_GRID], int gridSize, struct slot slots[], int slotIndex,
	struct word words[], int numWords, int wordIndex);
int task5CheckPlacement(char grid[][MAX_CW_GRID], int row, int column, char direction, char value[], int index);
void task5DoPlacement(char grid[][MAX_CW_GRID], int row, int column, char direction, char value[],
	int index, int write);
void task5ReWrite(char grid[][MAX_CW_GRID], struct slot slots[], struct word words[], int numWords, int index);

int main() {
	int task = -1;
	do {
		printf("Choose an option:\n"
			"1. Robot Paths\n"
			"2. The Human Pyramid\n"
			"3. Parenthesis Validation\n"
			"4. Queens Battle\n"
			"5. Crossword Generator\n"
			"6. Exit\n");

		if (scanf("%d", &task)) {
			switch (task) {
			case 6:
				printf("Goodbye!\n");
				break;
			case 1:
				int column = 0, row = 0;
				printf("Please enter the coordinates of the robot (column, row):\n");
				scanf("%d %d", &column, &row);
				printf("The total number of paths the robot can take to reach home is: %d\n",
				       task1RobotPaths(column, row));
				break;
			case 2:
				task2HumanPyramid();
				break;
			case 3:
				printf("Please enter a term for validation:\n");
				// If there is whitespace after the menu selection, clear it from buffer before calling function
				scanf("%*[ \n\t\v\f\r]");
				task3ParenthesisValidator();
				break;
			case 4:
				task4QueensBattle();
				break;
			case 5:
				task5CrosswordGenerator();
				break;
			default:
				printf("Please choose a task number from the list.\n");
				break;
			}
		}
		else {
			scanf("%*s");
		}
	}
	while (task != 6);
}

/* Recursive function where each time we will check both left-step and down-step. All the paths that will
 * reach out of bounds (negative values) will end and return 0. All the paths that will reach home will end
 * and return 1. Each time we will sum the results to get number of successful paths. */
int task1RobotPaths(int column, int row) {
	if (column < 0 || row < 0) {
		return 0;
	}
	if (column == 0 && row == 0) {
		return 1;
	}
	return task1RobotPaths(column - 1, row) + task1RobotPaths(column, row - 1);
}

// This function handles the input and output for task 2.
void task2HumanPyramid() {
	float weights[PYRAMID_SIZE][PYRAMID_SIZE] = {0};
	printf("Please enter the weights of the cheerleaders:\n");
	/* Nested loop to get weights input. Inner loop runs until index of outer loop in order to
	 * get 1 input for row 1, 2 inputs for row 2, and so on... */
	for (int i = 0; i < PYRAMID_SIZE; i++) {
		for (int j = 0; j <= i; j++) {
			scanf("%f", &weights[i][j]);
			if (weights[i][j] < 0.0) {
				printf("Negative weights are not supported.\n");
				return;
			}
		}
	}
	printf("The total weight on each cheerleader is:\n");
	// Nested loop to print weights output.
	for (int i = 0; i < PYRAMID_SIZE; i++) {
		for (int j = 0; j <= i; j++) {
			printf("%.2f ", task2WeightCalc(weights, PYRAMID_SIZE, i, j));
		}
		printf("\n");
	}
}

// This function handles the recursive weight calculation for task 2.
float task2WeightCalc(float weights[][PYRAMID_SIZE], int pyramidSize, int requestedRow, int requestedColumn) {
	// If we are out of array bounds - just return 0
	if (requestedRow < 0 || requestedRow >= pyramidSize || requestedColumn < 0 || requestedColumn >= pyramidSize) {
		return 0;
	}
	// If we are at the top of the pyramid - return the weight at the top
	if (requestedRow == 0 && requestedColumn == 0) {
		return weights[requestedRow][requestedColumn];
	}
	/* In any other condition - return half the weights in each side above us + our weight (note: the one on our right
	 * shoulder has the same column index as us, and the one on our left shoulder has our column index - 1 ). */
	float result = task2WeightCalc(weights, pyramidSize, requestedRow - 1, requestedColumn - 1) / 2;
	result += task2WeightCalc(weights, pyramidSize, requestedRow - 1, requestedColumn) / 2;
	return result + weights[requestedRow][requestedColumn];
}

// In task 3 we have two recursive functions. This one runs while there are no parenthesis currently open.
void task3ParenthesisValidator() {
	char currentChar;
	scanf("%c", &currentChar);
	// If we have reached \n with no open parenthesis, print balanced and return
	if (currentChar == '\n') {
		printf("The parentheses are balanced correctly.\n");
		return;
	}
	// If there is a closing parenthesis without an opening one, clear buffer, print not balanced and return
	if (currentChar == ')' || currentChar == ']' || currentChar == '}' || currentChar == '>') {
		scanf("%*[^\n]%*c");
		printf("The parentheses are not balanced correctly.\n");
		return;
	}
	// If there is a new opening parenthesis
	if (currentChar == '(' || currentChar == '[' || currentChar == '{' || currentChar == '<') {
		/* Call task3NewOpenParenthesis, if it returned 1, the parenthesis were closed correctly and we
		 * repeat the process recursively */
		if (task3NewOpenParenthesis(currentChar)) {
			task3ParenthesisValidator();
		}
		// If it returned 0 (parenthesis not closed correctly), then return (output was already handled).
		return;
	}
	// If none of the above happened - call recursively
	task3ParenthesisValidator();
}

/* This is the second recursive function for task 3. This one runs while there are parenthesis currently open.
 * It returns 1 if the parenthesis were closed correctly, else it returns 0 (in which case it also handles
 * buffer cleaning and user output). */
int task3NewOpenParenthesis(char openParenthesis) {
	char currentChar;
	scanf("%c", &currentChar);
	// If we have reached \n without closing parenthesis, clear buffer, print not balanced and return 0
	if (currentChar == '\n') {
		printf("The parentheses are not balanced correctly.\n");
		return 0;
	}
	// If there is a closing parenthesis
	if (currentChar == ')' || currentChar == ']' || currentChar == '}' || currentChar == '>') {
		// If the closing parenthesis matches the open one, return 1
		if ( (currentChar == ')' && openParenthesis == '(') || (currentChar == ']' && openParenthesis == '[')
			|| (currentChar == '}' && openParenthesis == '{') || (currentChar == '>' && openParenthesis == '<')) {
			return 1;
			}
		// If they don't match, clear buffer, print not balanced and return 0
		scanf("%*[^\n]%*c");
		printf("The parentheses are not balanced correctly.\n");
		return 0;
	}
	// If there is a new opening parenthesis, call this function recursively with the new parenthesis
	if (currentChar == '(' || currentChar == '[' || currentChar == '{' || currentChar == '<') {
		if ( ! task3NewOpenParenthesis(currentChar) ) {
			// If the new call returned 0, also return 0. Else - keep going.
			return 0;
		}
	}
	// If none of the above happened, call recursively
	return task3NewOpenParenthesis(openParenthesis);
}

// This is the general function for task 4, handles input and output
void task4QueensBattle() {
	char inputGrid[MAX_GRID][MAX_GRID] = {0}, areasList[MAX_GRID] = {0};
	int outputGrid[MAX_GRID][MAX_GRID] = {0}, occupiedRows[MAX_GRID] = {0}, size = 0; ;
	printf("Please enter the board dimensions:\n");
	scanf("%d", &size);
	printf("Please enter a %d*%d puzzle board:\n", size, size);
	// Nested loop to get input
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			scanf(" %c", &inputGrid[i][j]);
		}
	}
	// Call function to populate areasList array from input
	task4CreateAreasList(inputGrid, areasList, size, 0, 0, 0);
	/* Call placement function and check return value. If successful - print result. If not - print
	 * unsolvable. Note: We send -2 as initial 'previous-placement' parameter to indicate there was no previous
	 * placement as this is the first column. The reason -2 was chosen is that later it will be used to check that
	 * placement is different from (prevPlacement+1) so we need a number that even after +1 is not a real index. */
	if (task4PlacementColumns(inputGrid, outputGrid, occupiedRows, areasList, size, 0, 0, PREV_PLACEMENT_INIT)) {
		// Success - nested loop to print result grid output
		printf("Solution:\n");
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				char c = outputGrid[i][j] ? 'X' : '*';
				printf("%c ", c);
			}
			printf("\n");
		}
	} else {
		printf("This puzzle cannot be solved.\n");
	}
}

// Recursive function that creates a list of the different areas from the input grid
void task4CreateAreasList(char inputGrid[][MAX_GRID], char areasList[], int size, int row, int column, int areasAdded) {
	// If we have reached the end of row
	if (column >= size) {
		// If we have also reached the end of column
		if (row >= size) {
			return;
		}
		// Move to the start of next row
		task4CreateAreasList(inputGrid, areasList, size, row + 1 , 0, areasAdded);
		return;
	}
	// If we haven't added this area yet - add it
	if ( ! task4IsAreaInArray(areasList, size, 0, inputGrid[row][column]) ) {
		areasList[areasAdded] = inputGrid[row][column];
		areasAdded++;
	}
	// Call recursively to check next column
	task4CreateAreasList(inputGrid, areasList, size, row, column + 1, areasAdded);
}

// Recursive function that checks if a certain area exists in array. Returns 1 if exists, 0 if not.
int task4IsAreaInArray(char areasList[], int size, int startAt, char areaToCheck) {
	// If we reached the size and haven't found, return 0
	if (startAt >= size) {
		return 0;
	}
	// If the area to check exists in this index
	if (areasList[startAt] == areaToCheck) {
		return 1;
	}
	// If none of the above, call recursively to check next index and return result
	return task4IsAreaInArray(areasList, size, startAt + 1, areaToCheck);
}

/* First of two recursive functions that create the placements. This one goes column by column.
 * Returns 1 if column placement was successful, otherwise returns 0. */
int task4PlacementColumns(char inputGrid[][MAX_GRID], int outputGrid[][MAX_GRID], int occupiedRows[],
	char areasList[], int size, int column, int row, int prevPlacement) {

	// Call row placement function to find available row for this column and save result
	int rowResult = task4PlacementRows(inputGrid, occupiedRows, areasList, size, row, column, prevPlacement);
	// If successful (available row found)
	if (rowResult != -1) {
		// Place queen
		outputGrid[rowResult][column] = 1;
		// Mark row as occupied
		occupiedRows[rowResult] = 1;
		// If this was the last column - we are done, return success.
		if (column == size - 1) {
			return 1;
		}
		// Create a new updated area list to send to the next column
		char updatedAreasList[MAX_GRID];
		task4RemoveArea(areasList, updatedAreasList, size, 0, inputGrid[rowResult][column]);
		// Make recursive call to next column and check result
		if (task4PlacementColumns(inputGrid, outputGrid, occupiedRows, updatedAreasList,
			size, column + 1, 0, rowResult)) {
			return 1;
		}
		// Next column returned 0 - undo selection (remove queen, mark row as unoccupied).
		outputGrid[rowResult][column] = 0;
		occupiedRows[rowResult] = 0;
		// If the row we just tried was the last one, also return 0
		if (rowResult == size - 1) {
			return 0;
		}
		// Else - make recursive call to same column but this time start at the row after the one we just removed.
		return task4PlacementColumns(inputGrid, outputGrid, occupiedRows, areasList,
			size, column, rowResult + 1, prevPlacement);
	} else {
		// If no available row was found in this column
		return 0;
	}
}

/* Second of two recursive functions that create the placement. This one goes row by row within a column
 * to find available placement. If available placement was found - returns available row index. If no
 * available row was found - returns -1 (because 0 is a valid index). */
int task4PlacementRows(char inputGrid[][MAX_GRID], int occupiedRows[], char areasList[],
	int size, int row, int column, int prevPlacement) {

	// If we reached the size and haven't found, return -1.
	if (row >= size) {
		return -1;
	}
	/* Since we are going column by column, we need to check if the row is available and then only
	 * the 2 corners (previous placement +\- 1) for proximity. Then we will check if the area is free. */
	if (!occupiedRows[row] && row != (prevPlacement + 1) && row != (prevPlacement - 1)) {
		// Check that area is free
		if (task4IsAreaInArray(areasList, size, 0, inputGrid[row][column])) {
			// If all checks cleared, we can place queen here - return row number
			return row;
		}
	}
	// Make recursive call to check the next row and return the result
	return task4PlacementRows(inputGrid, occupiedRows, areasList, size, row + 1, column, prevPlacement);
}

// Recursive function to populate a new area list - without the area we want to remove.
void task4RemoveArea(char oldAreasList[], char newAreasList[], int size, int startAt, char areaToRemove) {
	// If we reached the end, stop
	if (startAt >= size) {
		return;
	}
	// As long as this is not the area to remove - copy it to new array.
	if (oldAreasList[startAt] != areaToRemove) {
		newAreasList[startAt] = oldAreasList[startAt];
	}
	else {
		newAreasList[startAt] = '\0';
	}
	// Recursive call to next index
	task4RemoveArea(oldAreasList, newAreasList, size, startAt + 1, areaToRemove);
}

// This is the general function for task 5. Handles input and output.
void task5CrosswordGenerator() {
	char grid[MAX_CW_GRID][MAX_CW_GRID] = {0};
	struct slot slots[MAX_SLOTS];
	struct word words[MAX_WORDS];
	int gridSize = 0, numSlots = 0, numWords = 0;
	printf("Please enter the dimensions of the crossword grid:\n");
	scanf("%d", &gridSize);
	printf("Please enter the number of slots in the crossword:\n");
	scanf("%d", &numSlots);
	printf("Please enter the details for each slot (Row, Column, Length, Direction):\n");
	for (int i = 0; i < numSlots; i++) {
		scanf("%d %d %d", &slots[i].row, &slots[i].column, &slots[i].length);
		scanf(" %c", &slots[i].direction);
	}
	printf("Please enter the number of words in the dictionary:\n");
	while (numWords < numSlots) {
		scanf("%d", &numWords);
		if (numWords < numSlots) {
			printf("The dictionary must contain at least %d words. Please enter a valid dictionary size:\n", numSlots);
		}
	}
	printf("Please enter the words for the dictionary:\n");
	for (int i = 0; i < numWords; i++) {
		scanf("%s", words[i].value);
		words[i].length = strlen(words[i].value);
		words[i].wasUsed = 0;
	}
	// Try to create crossword and check result
	if (task5FillSlots(grid, gridSize, slots, numSlots, words, numWords, 0, 0)) {
		// Success - print result
		for (int i = 0; i < gridSize; i++) {
			for (int j = 0; j < gridSize; j++) {
				if (grid[i][j] == 0) {
					printf("| # ");
				} else {
					printf("| %c ", grid[i][j]);
				}
			}
			printf("|\n");
		}
	} else {
		printf("This crossword cannot be solved.\n");
	}
}

// Recursive function that goes slot by slot and matches words. Returns 1 if successful, returns 0 otherwise.
int task5FillSlots(char grid[][MAX_CW_GRID], int gridSize, struct slot slots[], int numSlots,
	struct word words[], int numWords, int slotIndex, int wordIndex) {
	// If we filled all slots
	if (slotIndex >= numSlots) {
		return 1;
	}
	// Check if there is a match for this slot
	int matchResult = task5CheckMatch(grid, gridSize, slots, slotIndex, words, numWords, wordIndex);
	if ( matchResult != -1) {
		// Recursive call to match next slot, check result
		if (task5FillSlots(grid, gridSize, slots, numSlots, words, numWords, slotIndex + 1, 0)) {
			return 1;
		}
		// Next slot wasn't successful - mark word as unused and delete from grid
		words[matchResult].wasUsed = 0;
		words[matchResult].slotIndex = -1;
		task5DoPlacement(grid, slots[slotIndex].row, slots[slotIndex].column, slots[slotIndex].direction,
			words[matchResult].value, 0, 0);
		// When we delete, parts of other intersecting words might get deleted, so re-write them
		task5ReWrite(grid, slots, words, numWords, 0);
		// If the word we just tried was the last one - also return 0
		if (matchResult == numWords -1) {
			return 0;
		}
		// Else - recursive call to same slot - but this time start checking from next word
		return task5FillSlots(grid, gridSize, slots, numSlots, words, numWords, slotIndex, matchResult + 1);
	} else {
		// If no match was found
		return 0;
	}
}

/* Recursive function that tries to find matching word for given slot. If match was found - returns index
 * of matching word. If no match was found - returns -1. */
int task5CheckMatch(char grid[][MAX_CW_GRID], int gridSize, struct slot slots[], int slotIndex,
	struct word words[], int numWords, int wordIndex) {
	// If we checked all words
	if (wordIndex >= numWords) {
		return -1;
	}
	// Check if word wasn't used and if it's the same length as slot
	if (!words[wordIndex].wasUsed && words[wordIndex].length == slots[slotIndex].length) {
		// Check that the word doesn't clash with other words
		if (task5CheckPlacement(grid, slots[slotIndex].row, slots[slotIndex].column, slots[slotIndex].direction,
			words[wordIndex].value, 0)) {
			// Place word in slot on grid
			task5DoPlacement(grid, slots[slotIndex].row, slots[slotIndex].column, slots[slotIndex].direction,
				words[wordIndex].value, 0, 1);
			// Mark word as used
			words[wordIndex].wasUsed = 1;
			words[wordIndex].slotIndex = slotIndex;
			return wordIndex;
		}
	}
	// Word doesn't match - recursive call to check next word, and return result
	return task5CheckMatch(grid, gridSize, slots, slotIndex, words, numWords, wordIndex + 1);
}

/* Recursive function that checks if it's possible to input a word in a slot without clashing with other words.
 * Returns 1 on success, returns 0 otherwise. */
int task5CheckPlacement(char grid[][MAX_CW_GRID], int row, int column, char direction, char value[], int index) {
	// If we reached the end of the word
	if (value[index] == '\0') {
		return 1;
	}
	// If there is a clashing
	if (grid[row][column] != 0 && grid[row][column] != value[index]) {
		return 0;
	}
	// Call recursively to next index according to direction, and return the result
	if (direction == 'H') {
		return task5CheckPlacement(grid, row, column + 1, direction, value, index + 1);
	} else {
		return task5CheckPlacement(grid, row + 1, column, direction, value, index + 1);
	}
}

/* Recursive function that either writes or deletes a given word from a given slot in the grid, depending
 * on 'write' parameter (if 1 - write, if 0 - delete). */
void task5DoPlacement(char grid[][MAX_CW_GRID], int row, int column, char direction,
	char value[], int index, int write) {

	// If we reached the end of the word
	if (value[index] == '\0') {
		return;
	}
	// Write/delete character
	if (write) {
		grid[row][column] = value[index];
	} else {
		grid[row][column] = 0;
	}
	// Call recursively to next index according to direction, and return the result
	if (direction == 'H') {
		task5DoPlacement(grid, row, column + 1, direction, value, index + 1, write);
	} else {
		task5DoPlacement(grid, row + 1, column, direction, value, index + 1, write);
	}
}

// Recursive function that re-writes used words in case they were partly deleted
void task5ReWrite(char grid[][MAX_CW_GRID], struct slot slots[], struct word words[], int numWords, int index) {
	// If we reached the end
	if (index >= numWords) {
		return;
	}
	if (words[index].wasUsed) {
		// Rewrite word
		struct slot attachedSlot = slots[words[index].slotIndex];
		task5DoPlacement(grid , attachedSlot.row, attachedSlot.column, attachedSlot.direction,
			words[index].value, 0, 1);
	}
	// Recursive call to next word
	task5ReWrite(grid, slots, words, numWords, index + 1);
}