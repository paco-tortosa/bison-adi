#include <iostream>
#include <vector>
#include <functional>
#include <map>
#include <sstream>

using namespace std;
typedef std::function<double()> funExpr;

class Cell {
private:
public:
    vector<string> dependsOn;
    funExpr expr;

    Cell() {}

    Cell(double _dValue) {
        expr = [_dValue]() { return _dValue; };
    }

    Cell(const funExpr& _fExpression) {
        expr = _fExpression;
    }

    double operator()() {
        return expr();
    }

    void AddDependsOn(const string& cellName) {
        dependsOn.push_back(cellName);
    }
};

map<string /*cell*/, Cell> cellsAll;

bool CalculateAllCells(string& error) {
    map<string, Cell*> cellsReady;

    for (;;) {
        int cellsReadyCount = cellsReady.size();
        for (auto& a : cellsAll) {
            const string& cellName = a.first;
            if (cellsReady.count(cellName)) {
                //Cell is ready
                continue;
            }
            Cell& cell = a.second;
            if (cell.dependsOn.size() == 0) {
                cellsReady[cellName] = &cell;
                continue;
            }
            bool allDependenciesAreReady = true;
            for (auto& depCellName : cell.dependsOn) {
                if (cellsReady.count(depCellName) == 0) {
                    allDependenciesAreReady = false;
                    break;
                }
            }
            if (allDependenciesAreReady) {
                cellsReady[cellName] = &cell;
                continue;
            }
        }
        if (cellsReady.size() == cellsReady.size())
            break;
        if (cellsReadyCount == cellsReady.size())
            break;
    }
    bool success = cellsReady.size() == cellsAll.size();
    if (!success) {
        stringstream ss;
        ss << "The following cells cannot be calculated: ";
        int notReadyCells = 0;
        for (auto& a : cellsAll) {
            if (cellsReady.count(a.first) == 0) {
                if (notReadyCells) {
                    ss << ", ";
                }
                notReadyCells++;
                ss << a.first;
            }
        }
        error = ss.str();
    }
    return success;
}

int main()
{
    cellsAll["A3"] = Cell(10);
    cellsAll["A3"].AddDependsOn("B1");
    cellsAll["A1"] = Cell(8);
    cellsAll["A4"] = Cell( static_cast<funExpr>([]() { return 5; }) );
    cellsAll["B1"] = Cell( static_cast<funExpr>([]() { return 10 * cellsAll["A3"]() + cellsAll["A1"]();}) );
    cellsAll["B1"].AddDependsOn("A3");
    cellsAll["B1"].AddDependsOn("A1");

    string error;
    if (CalculateAllCells(error)) {
        for (auto& a : cellsAll) {
            cout << a.first << " = " << a.second() << endl;
        }
    }
    else {
        cout << error << endl;
    }
}
