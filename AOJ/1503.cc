//Name: Numbers
//Level: 3
//Category: 素数,ポラード・ロー法
//Note:

/*
 * 1から1500が含んでいる素因数のうち，それぞれ最大のものを選んでいくとその積は合成数であり，
 * n+2, n+3, ... n+1500 は全て合成数になる．
 * ここで n+1 が合成数かどうかが問題となるが，Ruby の prime を使うと素数判定ができる．
 * （おそらくミラーラビン）
 * n+1を判定するのは時間がかかるので n*2+1, n*3+1, ... を調べると，n*3+1 は即座に合成数判定が出る．
 * あとはこれをポラード・ロー法によって素因数分解すると，素因数 22133 が得られる．
 */
#include <iostream>

using namespace std;

int main() {
    int N;
    cin >> N;
    cout << "88874100321238209719819013272565736216284761125002371810653300184015543867497208821304903291591893742690314210628661444749205508727880023679928585142957605520362472512545133643763317352169701404401216360248053883700606827872818945761708984895130149801381566852502381938413035090108099910195416469364225372303530346759237976066080348509785111131341216038505598381132070513707706447467014464030041516012813927795482817537692315535275898685085658868825647710646479438286705396164993046832226271418944049587951088309367848535540055794386932605551180265087442965893429828359883445648307784394602905061553039148922250421325882787365587821737028864469560718720000" << endl;
    for(int i = 0; i < N; ++i) {
        if(i == 0) cout << 2 << endl;
        else if(i == 1) cout << 22133 << endl;
        else cout << i << endl;
    }
    return 0;
}
