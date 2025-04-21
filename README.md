Program kompresujący i dekompresujący pliki z użyciem [kodowania Huffmana](https://pl.wikipedia.org/wiki/Kodowanie_Huffmana).

# Instrukcja

## Komendy

- `encode <plik wejściowy 1> <plik wejściowy 2> ...`  
  Kompresuje podane pliki do archiwum.

- `decode <nazwa pliku archiwum do rozpakowania>`  
  Rozpakowuje wskazane archiwum.

## Opcje

- `-o <nazwa pliku>`  
  Określ nazwę pliku wyjściowego (archiwum). Tylko dla `encode`.

- `-d`  
  Wypisuj dodatkowe informacje do debugowania.

- `-s`  
  Wypisz statystyki. Tylko dla `encode`.

- `-h`  
  Wyświetl pomoc.

- `-p <ścieżka>`  
  Określ ścieżkę, gdzie „wypakować” archiwum. Tylko dla `decode`.
