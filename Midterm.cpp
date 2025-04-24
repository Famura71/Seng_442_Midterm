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
    int n = Array_Length / thread_no;//n = Thread başına düşen liste uzunluðu. Global veri olması daha optimal ama sınavda fonksyonun içine yazdıðım için buraya bıraktım
    int* temp = new int[n];//Thread listesi
    for (int z = 0; z < n; z++) {
        temp[z] = My_Array[((thread_id+1) * n)-(z+1)];//Numarası bir fazla olan thread'in ilk verisinin bir eksiðinden(bizim listemizin son elemanından) başlayarak listemizi azalan düzende temp'e yaz
    }
    sync_point.arrive_and_wait();//Tüm threadlerin önceki yazma işlemini yapmasını bekle(barrier)

    for (int q = 0; q < n; q++) {
        My_Array[((thread_no - (thread_id+1)) * n) + q] = temp[q];//Threadlerin iç listelerini, threadlerin numaralarının tersine göre(Örnek: ilk thread'i son thread yerine, son threadi ilk thread yerine) geri global array'e yaz.
    }
    delete[] temp;//Memory leak'i engellemek için temp'i sil, algoritma ile alakası yok
}

int main()
{   
    std::cout << Array_Length << std::endl;//Kontrol etmek için Array uzunluðunu yazdır
    std::cout << " " << std::endl;//Araya boşluk koy
    for (int i = 0; i < Array_Length; i++) {
        std::cout << My_Array[i] << std::endl;//Yine kontrol etmek için array'in tüm elemanlarını yazdır
    }
    std::cout << " " << std::endl;//Araya boşluk koy
    std::vector<std::thread> threads;//Oluşacak thread'leri sonradan birleştirebilmek için vektör içine kaydet
    for (int x = 0; x < thread_no; x++) {
        threads.emplace_back(reverse, x);//Threadleri oluştur
    }

    for (auto& t : threads) {
        t.join();//Threadleri birleştir
    }
    for (int i = 0; i < Array_Length; i++) {
        std::cout << My_Array[i] << std::endl;//Threadlerin deðiştirmiş olduðu array'i yazdır
    }
    return 0;
}
