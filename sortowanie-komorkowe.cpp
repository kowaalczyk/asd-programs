/*
 * Próbujemy w sprytny sposób zrobić rekonstrukcję ciągów które mogłyby się do danego przesortować:
 * 1. bierzemy ostatni element
 * 2. Przykładowo dla 562143:
 *      - zakładamy pewną liczbę która była na końcu, np.4
 *          - dopisujemy liczby wg. algorytmu sortowania
 *          - zostajemy na koniec z 4 możliwościami: 52, 25, 12, 21
 *      - powtarzamy dla pozostałych możliwych cyfr na końcu
 *      - gdy wybierzemy sobie 3, zostajemy ponownie z 4 na ustalonym miejscu i cyframi 1,2,5 do wstawienia
 *          - ponieważ zastanawiamy się ile możliwych ciągów moglibyśmy uzyskać (nie interesują nas konkretne ciągi tylko ich liczba)
 *          - możemy zapamiętywać te liczby (konkretnie: ile prefixów tworzy dany przedział w danym ciągu) dynamicznie (np. wracając) -> pamięć n^2 czyli OK, czas działania: n^2
 *          - to jest optymalne rozwiązanie ale trudne w implementacji bez boosta
 *          - prościej jest zacząć samemu budować cache:
 *              - najpierw zapamiętać wszystkie 1-elementowe przedziały i stwierdzić że będą powstawały tylko z 1-elementowych prefixów
 *              - dla 2-el. mamy kończący się z lewej strony i kończący się z prawej strony
 *              - dla n liczbowych: z ilu prefixów można utworzyć [i, ..., n+i-1]? Trzeba spojrzeć na 2 ostatnie przedziały, zobaczyć które z nich sa możliwe w danej sytuacji i zsumować możliwe liczby
 * 3. Wszystkie działania robimy mod 10^9
 *
 * -- Dynamiczne rozwiązanie dla 562143
 * - mamy 5 idących w prawo i 5 idących w lewo przedziałów 2-elementowych
 * - w prawą 31, 64 nie mogą powstać - cache: 0 1 1 1 0
 * - w lewą: 0 1 1 1 0
 * - 3-liczbowe: >>: 0, 2, 2, 0 (4 nie może być ani po 6 ani po 5) <<: 0, 2, 2(może być po 5 i po 6 więc sumujemy obie 1), 0(nie może być po 4, a po pozostałej cyfrze wychodz 0)
 * - itd.
 */