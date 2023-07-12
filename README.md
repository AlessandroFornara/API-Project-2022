# [Algorithms and Data Structures](https://www4.ceda.polimi.it/manifesti/manifesti/controller/ManifestoPublic.do?EVN_DETTAGLIO_RIGA_MANIFESTO=EVENTO&c_insegn=086067&aa=2021&k_cf=225&k_corso_la=358&ac_ins=0&k_indir=IT1&lang=EN&tipoCorso=ALL_TIPO_CORSO&semestre=2&idItemOfferta=155412&idRiga=268925&codDescr=086067) 2022, Course Final Project
This project is the final test of the "Algoritmi e Principi dell'Informatica" course at the Polytechnic of Milan, A.Y. 2021/22.

Evaluation: 30 with Honors / 30

- [Project Description](#project-description)
- [How to play](#how-to-play)
- [Implementations](#implementations)
- [Tools Used](#tools-used)
- [Software Used](#software-used)

## Project Description
The aim of this project is to apply the knowledge acquired during the course to recreate in C [Wordle](https://en.wikipedia.org/wiki/Wordle), a game whose objective is to guess a word through several attempts.
The project is evaluated on memory efficiency and speed.

The project, in detail, consists of the realisation of a system that checks the correspondence between the letters of two words of equal length.

In each game, there is a word to be guessed. After each attempt, a sequence of symbols (composed of +, | or /) is printed, which gives us information about each character in the attempt. In the symbol sequence there is + if the character in the corresponding position of the attempt is in the correct position, there is | if that character is present in the word to be guessed but in the wrong position, / if that character is not present at all in the word to be guessed.

## How to Play
Once started, the system reads from standard input:
- a k value indicating the length of the words
- a sequence (of arbitrary length) of words, each with length k, constituting the set of admissible words

After this, a sequence of matches is read, each starting with the `+nuova_partita` command.
The input of a match consists of:
- the word to be guessed (long k)
- maximum number n of attempts
- sequence of attempts (each long k)

During a game (between attempts) the `+stampa_filtrate` command may appear, this consists of printing in lexicographic order all the words compatible with the constraints learnt from the various attempts.

Both during and between games, the commands `+inserisci_inizio` and `+inserisci_fine` may appear. Between these commands there is a sequence of new words, which are added to the set of permissible words.

A game ends if the word is guessed after which it is printed ok, or if the last attempt is also wrong after which it is printed ko.

For full explanation see [pdf](/Specs/ENG.pdf) (also available in [Italian](/Specs/ITA.pdf)).


## Implementations
This project is implemented in C11 using only the [standard C libraries](https://en.wikipedia.org/wiki/C_standard_library).

I used a BST with a list. I save each word, as they are entered, in a node. I use the list to scroll through only the words that respect the constraints. After each attempt, if I learn new constraints, I throw out the words that do not respect them.

```c
typedef struct nodo_{
    char ok;
    struct nodo_* filt;
    struct nodo_* left;
    struct nodo_* right;
    char word[];
}RB_node;
```

## Tools used
- Valgrind;
- Callgrind;
- Massif-Visualizer;
- Address-Sanitizer;
- GDB;
- GCC.

## Software used
- Clion;
- Visual Studio Code.
