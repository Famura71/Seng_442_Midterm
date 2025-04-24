#include <iostream>
#include <thread>
#include <mutex>
#include <barrier>
#include <vector>

//Scenario 1:
/*
int My_Array[] = {1,2,3,4,5,6,7,8,9,10,11,12};
int thread_no = 3;
*/

//Senaryo 2:

int My_Array[] = {1,2,3,4,5,6,7,8,9,10};
const int thread_no = 5;


//Senaryo 3:
/*
int My_Array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60};
int thread_no = 10;
*/


int Array_Length = sizeof(My_Array) / sizeof(My_Array[0]);
std::barrier sync_point(thread_no);

void reverse(int thread_id) {
    int n = Array_Length / thread_no;//n = Thread ba��na d��en liste uzunlu�u. Global veri olmas� daha optimal ama s�navda fonksyonun i�ine yazd���m i�in buraya b�rakt�m
    int* temp = new int[n];//Thread listesi
    for (int z = 0; z < n; z++) {
        temp[z] = My_Array[((thread_id+1) * n)-(z+1)];//Numaras� bir fazla olan thread'in ilk verisinin bir eksi�inden(bizim listemizin son eleman�ndan) ba�layarak listemizi azalan d�zende temp'e yaz
    }
    sync_point.arrive_and_wait();//T�m threadlerin �nceki yazma i�lemini yapmas�n� bekle(barrier)

    for (int q = 0; q < n; q++) {
        My_Array[((thread_no - (thread_id+1)) * n) + q] = temp[q];//Threadlerin i� listelerini, threadlerin numaralar�n�n tersine g�re(�rnek: ilk thread'i son thread yerine, son threadi ilk thread yerine) geri global array'e yaz.
    }
    delete[] temp;//Memory leak'i engellemek i�in temp'i sil, algoritma ile alakas� yok
}

int main()
{   
    std::cout << Array_Length << std::endl;//Kontrol etmek i�in Array uzunlu�unu yazd�r
    for (int i = 0; i < Array_Length; i++) {
        std::cout << My_Array[i] << std::endl;//Yine kontrol etmek i�in array'in t�m elemanlar�n� yazd�r
    }
    std::cout << " " << std::endl;//Araya bo�luk koy
    std::vector<std::thread> threads;//Olu�acak thread'leri sonradan birle�tirebilmek i�in vekt�r i�ine kaydet
    for (int x = 0; x < thread_no; x++) {
        threads.emplace_back(reverse, x);//Threadleri olu�tur
    }

    for (auto& t : threads) {
        t.join();//Threadleri birle�tir
    }
    for (int i = 0; i < Array_Length; i++) {
        std::cout << My_Array[i] << std::endl;//Threadlerin de�i�tirmi� oldu�u array'i yazd�r
    }
    return 0;
}