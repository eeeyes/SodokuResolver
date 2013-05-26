#include <stdio.h>
#include <stdint.h>
typedef struct SodokuCell
{
	int8_t currentNumber;//-1 for no possible value,0 for not select
	int8_t possibleValues[9];
	int8_t possibleValuesCount;
}SodokuCell;

SodokuCell table[9][9];//x y

typedef struct SodokuSelection
{
	int8_t cellCoordinate[2];
	int8_t selectionValue;
	int8_t atPossibleIndex;
}SodokuSelection;

SodokuSelection stack[81];
int stackTop=-1;

void initSodokuTable();
int findAllPossibles(SodokuSelection* newSelection);
void pushSelection(SodokuSelection* selection);
int validate();
void popSelection();
int changeToNextSelection();
int isFull();
SodokuSelection* topSelection();
void outputSolution();
void findSolution();

void findSolution()
{
	initSodokuTable();
	int shouldFindNewSelection=1;
	while(1)
	{
		//generate a new selection
		int oldShouldFindNewSelection=shouldFindNewSelection;
		shouldFindNewSelection=1;
		int endOfSelection=0;
		if(oldShouldFindNewSelection)
		{
			SodokuSelection newSodokuSelection;
			int result=findAllPossibles(&newSodokuSelection);
			if(result){
				SodokuCell* cell=&table[newSodokuSelection.cellCoordinate[0]][newSodokuSelection.cellCoordinate[1]];
				newSodokuSelection.atPossibleIndex=0;
				newSodokuSelection.selectionValue=cell->possibleValues[newSodokuSelection.atPossibleIndex];	
				pushSelection(&newSodokuSelection);
			}
			else
			{
				printf("find process terminated\n");
				break;
			}
		}
		else
		{
			endOfSelection=!changeToNextSelection();
		}
		
		//check the solution
		if(validate())
		{
			printf("is validate\n");
			//outputSolution();
			if(isFull())
			{
				outputSolution();
				return;
				if(endOfSelection){
					popSelection();
				}	
				
				shouldFindNewSelection=0;
			}
			continue;
		}
		else
		{
			outputSolution();
			if(endOfSelection)
				popSelection();
			printf("not validate\n");
			shouldFindNewSelection=0;	
		}
	}
}
SodokuSelection* topSelection()
{
	if(stackTop==-1)
		return NULL;
	return stack+stackTop;
}
int isFull()
{
	int full=1;
	for(int y=0;y<9;y++)
		for(int x=0;x<9;x++)
		{
			if(table[x][y].currentNumber<=0){
				full=0;
				return full;
			}
		}
	return full;
}
void outputSolution()
{
	printf("=========================\n");
	for(int y=0;y<9;y++)
	{
		printf("|");
		for(int x=0;x<9;x++)
			printf("%d|",table[x][y].currentNumber);
		printf("\n");
	}
	printf("========================\n");
}
int changeToNextSelection()
{
	SodokuSelection* top=topSelection();
	if(!top)
		return 0;
	int currentPossibleIndex=top->atPossibleIndex;
	int x=top->cellCoordinate[0];
	int y=top->cellCoordinate[1];
	int possibleLength=table[x][y].possibleValuesCount;
	currentPossibleIndex++;
	top->atPossibleIndex=currentPossibleIndex;
	if(currentPossibleIndex>=possibleLength){
		return 0;
	}
	top->selectionValue=table[x][y].possibleValues[currentPossibleIndex];
	table[x][y].currentNumber=top->selectionValue;
	return 1;
}
void popSelection()
{
	if(stackTop==0)
		return;
	SodokuSelection* selection=stack+stackTop;
	int x=selection->cellCoordinate[0];
	int y=selection->cellCoordinate[1];
	//change stack
	stackTop--;
	//change table
	table[x][y].currentNumber=0;
}
int validate()
{
	SodokuSelection* top=topSelection();
	if(!top)
		return 0;
	int x=top->cellCoordinate[0];
	int y=top->cellCoordinate[1];
	SodokuCell* cell=&table[x][y];
	if(cell->possibleValuesCount==top->atPossibleIndex)
		return 0;
	for(int i=0;i<9;i++)
	{
		int countOnX[9],countOnY[9];
		for(int k=0;k<9;k++)
		{
			countOnX[k]=0;
			countOnY[k]=0;
		}
		for(int j=0;j<9;j++)
		{
			int currentNumberOnX=table[i][j].currentNumber;
			int currentNumberOnY=table[j][i].currentNumber;

				
			if(currentNumberOnX>0&&((++countOnX[currentNumberOnX-1]))>1)
				return 0;
			if(currentNumberOnY>0&&((++countOnY[currentNumberOnY-1]))>1)
				return 0;

			
		}
	}


	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
		{
			int count[9];
			for(int k=0;k<9;k++)
			{
				count[k]=0;
			}
			int startX=i*3;
			int startY=j*3;
			for(int x=startX;x<startX+3;x++)
				for(int y=startY;y<startY+3;y++)
				{
					int currentNumber=table[x][y].currentNumber;
					if(currentNumber<=0)
						continue;
					if((++count[currentNumber-1])>1)
						return 0;
				}
		}
	return 1;
}
void pushSelection(SodokuSelection* selection)
{
	//change the stack
	SodokuSelection* currentSelection=stack+(++stackTop);
	printf("current index %d with value %d\n",stackTop,selection->selectionValue);
	currentSelection->selectionValue=selection->selectionValue;
	currentSelection->atPossibleIndex=selection->atPossibleIndex;
	currentSelection->cellCoordinate[0]=selection->cellCoordinate[0];
	currentSelection->cellCoordinate[1]=selection->cellCoordinate[1];
	//change the table
	SodokuCell* cell=&table[currentSelection->cellCoordinate[0]][currentSelection->cellCoordinate[1]];
	cell->currentNumber=currentSelection->selectionValue;
}
void initSodokuTable()
{
	for(int i=0;i<9;i++)
		for(int j=0;j<9;j++)
		{
			table[i][j].currentNumber=-1;
		}
	
	table[0][0].currentNumber=2;
	table[2][0].currentNumber=6;
	table[5][0].currentNumber=1;
	table[7][0].currentNumber=8;

	table[0][1].currentNumber=1;
	table[1][1].currentNumber=7;
	table[5][1].currentNumber=9;
	table[7][1].currentNumber=6;

	table[3][2].currentNumber=4;
	table[4][2].currentNumber=6;
	table[5][2].currentNumber=7;
	
	table[0][3].currentNumber=6;
	table[1][3].currentNumber=1;
	table[4][3].currentNumber=4;
	table[6][3].currentNumber=8;

	table[2][4].currentNumber=2;
	table[6][4].currentNumber=3;
	
	table[2][5].currentNumber=5;
	table[4][5].currentNumber=7;
	table[7][5].currentNumber=9;
	table[8][5].currentNumber=6;


	table[3][6].currentNumber=2;
	table[4][6].currentNumber=1;
	table[5][6].currentNumber=5;

	table[1][7].currentNumber=3;
	table[3][7].currentNumber=6;
	table[7][7].currentNumber=2;
	table[8][7].currentNumber=8;

	table[1][8].currentNumber=2;
	table[3][8].currentNumber=7;
	table[6][8].currentNumber=6;
	table[8][8].currentNumber=5;
}
int findAllPossibles(SodokuSelection* newSelection)
{
	int8_t selectedCoordinate[2]={-1,-1};
	int currentSmallestCount=10;
	for(int x=0;x<9;x++)
		for(int y=0;y<9;y++)
		{
			SodokuCell* cell=&table[x][y];
			int currentNumber = cell->currentNumber;
			int possibleCount = cell->possibleValuesCount;
			//find all possibles when not
			if(currentNumber<0)
			{
				cell->currentNumber=0;
				int possibleValue[9];
				for(int i=0;i<9;i++)
					possibleValue[i]=1;
				//remove on x
				for(int i=0;i<9;i++)
				{
					int currentNumber=table[x][i].currentNumber;
					if(currentNumber<=0)
						continue;
					possibleValue[currentNumber-1]=0;
				}
				
				//remove on y
				for(int i=0;i<9;i++)
				{
					int currentNumber=table[i][y].currentNumber;
					if(currentNumber<=0)
						continue;
					possibleValue[currentNumber-1]=0;
				}

				//remove in block
				int sectionX=x/3;
				int sectionY=y/3;
				
				for(int sx=sectionX*3;sx<(sectionX+1)*3;sx++)
					for(int sy=sectionY*3;sy<(sectionY+1)*3;sy++)
					{
						int currentNumber=table[sx][sy].currentNumber;
						if(currentNumber<=0)
							continue;
						possibleValue[currentNumber-1]=0;
					}

				int currentPossibleCount=0;
				printf("--------------");
				for(int i=0;i<9;i++)
				{
					if(possibleValue[i]==0)
						continue;
					cell->possibleValues[currentPossibleCount]=i+1;	
					currentPossibleCount++;
					printf("%d ",i+1);
				}
				cell->possibleValuesCount=currentPossibleCount;
				printf("\n---------------\n");
				if(currentPossibleCount==0)
				{
					return 0;
				}
			}
			if(currentNumber<=0)
			{
				possibleCount=cell->possibleValuesCount;
				if(currentSmallestCount>possibleCount)
				{
					currentSmallestCount=possibleCount;
					selectedCoordinate[0]=x;
					selectedCoordinate[1]=y;
					printf("x:%d y:%d\n",x,y);
				}
			}	
		}
	if(selectedCoordinate[0]<0)
	{
		return 0;
	}
	newSelection->cellCoordinate[0]=selectedCoordinate[0];
	newSelection->cellCoordinate[1]=selectedCoordinate[1];
	return 1;
}
int main(int argc,char*argv[])
{
	printf("welcome to sokoku reslover\n");

	findSolution();
	
	return 0;
}
