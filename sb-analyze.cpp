//Name: Parker Babb
//Project3 Part - A 

#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "disjoint.h"

using namespace std;

class Superball {
  public:
    Superball(int argc, char **argv);
	void analyze_superball();
    int r;
    int c;
    int mss;
    int empty;
    vector <int> board;
    vector <int> goals;
    vector <int> colors;
};

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-analyze rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int) strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);

  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if ((int) s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
    }
  }
}

void Superball::analyze_superball() { 

	int n = r * c;
	DisjointSetByRankWPC ds(n); 

	//Merge neighboring cells of same color. Checking left and upper neighbors redundant
	for (int i = 0; i  < r; i++) { 
		for (int j = 0; j < c; j++) {

			//Google for common ways of converting 2d index -> 1d
			int idx = i*c +j;
			if (board[idx] == '.' || board[idx] == '*') {
				continue;
			}

			//Check right neighbor if same color, merge if not in same group
			if (j + 1 < c && board[idx + 1] == board[idx]) { 
				int a = ds.Find(idx);
				int b = ds.Find(idx + 1); 
				if (a != b) { 
					ds.Union(a,b);
				}
			}

			//Check lower neighbor, merge if same color
			if (i + 1 < r && board[idx + c] == board[idx]) { 
				int a = ds.Find(idx);
				int b = ds.Find(idx + c); 
				if (a != b) { 
					ds.Union(a, b);
				}
			}
		}
	}


	//Determine group size and if it touches a scoring cell
	vector <int> sizes(n, 0);
	vector <int> goalcell(n, -1);

	for (int idx = 0; idx < n; idx++) { 
		if (board[idx] == '.' || board[idx] == '*') { 
			continue;
		}
		int root = ds.Find(idx);
		sizes[root]++;
		if (goals[idx] && goalcell[root] == -1) { 
			goalcell[root] = idx;
		}
	}

	//Print all groups big enough with scoring cell
	printf("Scoring sets:\n");
	for (int root = 0; root < n; root++) { 
		if (sizes[root] >= mss && goalcell[root] != -1) { 
			printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n",
					sizes[root],
					board[root],
					goalcell[root] / c,
					goalcell[root] % c);
		}
	}
}


int main(int argc, char **argv)
{
  Superball *s;
 
  s = new Superball(argc, argv);
  s->analyze_superball();
  
  
}
