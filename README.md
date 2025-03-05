# System powiadomień oparty na kolejkach komunikatów

## Opis projektu
Projekt zrealizowany na studia na przedmiot programowanie systemowe i współbieżne. System powiadomień oparty na kolejkach komunikatów umożliwia przekazywanie powiadomień od wielu procesów producenta do centralnego dyspozytora, który następnie rozsyła je do subskrybujących klientów.

## Cel projektu
Celem projektu jest implementacja systemu przekazywania powiadomień, w którym:
- Producent generuje i wysyła powiadomienia (np. alerty, aktualizacje) do dyspozytora.
- Dyspozytor zarządza subskrypcjami i przekazuje wiadomości od producenta do klientów.
- Klient otrzymuje powiadomienia i wyświetla je na ekranie.

## Jak uruchomić aplikację
1. Otwórz terminal w katalogu projektu.
2. Użyj kompilatora `gcc` do skompilowania plików źródłowych:
    ```sh
    gcc -o dispatcher dispatcher.c -lrt
    gcc -o producer producer.c -lrt
    gcc -o client client.c -lrt
    ```
3. Uruchom odpowiednie pliki wykonywalne:
    ```sh
    ./dispatcher
    ./producer <producer_id>
    ./client <client_id>
    ```

## Wymagania systemowe
- System operacyjny: Linux
- Kompilator C (np. `gcc`)
- Terminal lub konsola

## Instrukcja instalacji
1. Upewnij się, że masz zainstalowany kompilator `gcc`.
2. Sklonuj repozytorium projektu na swój lokalny komputer:
    ```sh
    git clone https://github.com/CByDesign/Notification-System/
    ```
3. Przejdź do katalogu projektu:
    ```sh
    cd NotificationSystem
    ```
4. Skompiluj pliki źródłowe:
    ```sh
    gcc -o dispatcher dispatcher.c -lrt
    gcc -o producer producer.c -lrt
    gcc -o client client.c -lrt
    ```

## Przykłady użycia
- Uruchomienie dyspozytora:
    ```sh
    ./dispatcher
    ```
- Wysłanie powiadomienia przez producenta:
    ```sh
    ./producer <producer_id>
    Wybranie odpowiedniej opcji w menu
    ```
- Subskrypcja klienta na powiadomienia typu "alert":
    ```sh
    ./client <client_id>
    Wybranie odpowiedniej opcji w menu
    ```

## License

This project is under the MIT License - see the [LICENSE](./LICENSE) file for details.