#include<iostream>

using namespace std;

int mapp[4][4];// арта, котора€ обрабатываетс€ роботом(его карта)
const int varX[] = {1, 0, -1, 0 };
const int varY[] = {0, 1,  0, -1}; 

bool chek=false;
int main()
{
	int coordX = 3, coordY =1;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			mapp[i][j]=1;
	mapp[1][3] = -2;
	int used[4][4];
  int search[4][4];
  int used2[4][4];
  int q = 0;
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
    {
      if(mapp[i][j]!=1)search[i][j] = -1;
      else search[i][j] = 0;
    }
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      used[i][j] = search[i][j]; 
  search[coordX][coordY]=0;
  used[coordX][coordY]=2;
  while(true)
  {
	  chek = false;
    q++;
    for(int i = 0; i < 4; i++)
      for(int j = 0; j < 4; j++)
      {
        if(used[i][j]==2)
        {
			chek=true;
          for(int w = 0; w < 4; w++)
          {
            if(i+ varX[w] < 4 && i + varX[w] > -1 && j+ varY[w] < 4 && j + varY[w] > -1 && used[i + varX[w]][j + varY[w]] == 0)
            {
              search[i + varX[w]][j + varY[w]] = q;
              used2[i + varX[w]][j + varY[w]] = 2;
            } 
          }
          used[i][j] = -1;
        }
      }
	  for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(used2[i][j]==2)
			{
				used[i][j]=2;
				used2[i][j]=0;
			}
		}

/*			cout << "\n\n";
			 for(int i = 0; i < 4; i++)
			 {
		for(int j = 0; j < 4; j++)
			cout << search[i][j];
		cout << "\n";
			 }*/
	  if(!chek)break;
  }
  cout << "\n\n\n\n";
  for(int i = 0; i < 4; i++)
  {
      for(int j = 0; j < 4; j++)
		  cout << search[i][j];
	  cout << "\n";
  }
  system("pause");
}