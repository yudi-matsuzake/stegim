# Opções do stegim

Nesse arquivo conterá TODAS as opções que estão sendo implementadas e/ou ideias de opções a serem implementadas.

## STEGIM
---------

Opções do programa sem comandos ou que precedem os comandos.

`--verbosity` `-v`	Habilita o modo verboso

`--quiet` `-q`	Desabilita o modo verboso

## DEFAULT

Opções padrões dos comandos, i.e., opções que conterão em todos os comandos.

`--channels=BGR_STRING` `-C BGR_STRING` Escolha de canais que serão usados para esconder o texto.

* *Exemplos de BGR_STRING*

* `"BGR"` - Todos os canais serão usados;

* `"BG-"` - Somente o canal Azul e Verde serão usados;

* `"--R"` - Somente o canal vermelho será usado;

* `"---"` - Nenhum canal será usado (isso causará um erro).

`--nleast-siginificant-bit=N` `-b N` Número de bits menos significativos que serão usados para a máscara de esteganografia.

## STEG
-------

Opções do comando STEG

`--foreach-img-txt [img.jpg text.txt]+` `-F [img.jpg text.txt]+` Para cada imagem, use esse arquivo de entrada/saída;

## INFO
-------

Opções do comando INFO

## X
----

Opções do comando X

`--foreach-img-txt [img.jpg text.txt]+` `-F [img.jpg text.txt]+` Para cada imagem, use esse arquivo de entrada/saída;
