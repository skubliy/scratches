#include "h1.h"

using namespace std;

void calcDistance(vector < vector < int > >& city, int y, int x) {
    y = y - 1;
    x = x - 1;
    for (int i = 0; i < city[0].size(); i++)//x
        for (int j = 0; j < city.size(); j++)//y
        {
            if (city[j][i] != 0) {
                int old_v = city[j][i];
                int new_v = abs(j - y) + abs(i - x);
                if (new_v < old_v) city[j][i] = new_v;
            }
        }
}

vector < vector < int > > getLockerDistanceGrid(int cityLength, int cityWidth, vector < int >& lockerXCoordinates, vector < int >& lockerYCoordinates) {

    vector < vector < int > > v;
    v.reserve(cityLength);
    for (vector<vector < int > >::iterator it = v.begin(); it != v.end(); ++it) {//x
        vector < int >& vv = *it;
        vv.reserve(cityWidth);
        for (vector<int>::iterator itx = vv.begin(); itx != vv.end(); ++itx)
            *itx = 10000000;
    }
    //mark the locker's distance. it also remembers the location of lockers

    for (int i = 0; i < lockerXCoordinates.size(); i++)
        v[lockerYCoordinates[i] - 1][lockerXCoordinates[i] - 1] = 0; //calculate for each locker 

    for (int i = 0; i < lockerXCoordinates.size(); i++) {
        calcDistance(v, lockerYCoordinates[i], lockerXCoordinates[i]);
    }


    return v;
}

void print(vector < vector < int > > v) {

    for (vector<vector < int > >::iterator it = v.begin(); it != v.end(); ++it) {//y
        vector < int >& vv = *it;
        for (vector<int>::iterator itx = vv.begin(); itx != vv.end(); ++itx) {
            cout << *itx;
        }
    }
    cout << endl;
}

int main(int argc, char** argv) {
    int ax[] = {2, 4};
    size_t szx = sizeof (ax) / sizeof (*ax);
    vector<int> vx(ax, ax + szx);

    int ay[] = {1, 2};
    size_t szy = sizeof (ay) / sizeof (*ay);
    vector<int> vy(ay, ay + szy);

    print(getLockerDistanceGrid(5, 5, vx, vy));
    /*
        int[] x2 = {2, 4};
        int[] y2 = {3, 7};
        print(getLockerDistanceGrid(5, 7, x2, y2));
     */
    return 0;
}

