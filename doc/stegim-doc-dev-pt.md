# stegim

Programa simples para esteganografia em imagens.

## Overview
-----------

* A princípio não vamos utilizar criptografia nem compressão;

* Vai se basear em alterar os bits menos significativos dos canais da imagem;

* Os primeiro 4 bytes alterados vai ser um inteiro indicando quantos caracteres esteganografados existem na imagem;

## Linguagem

* Comentários: português

* Strings no programa: inglês

* README.md: inglês

## Comandos principais
----------------------

O programa vai receber um comando por execução, ele terá 3 funções principais:

* *steg*

* *info*

* *x*

### steg
----------

A função steg é a função que irá esteganografar os textos nas imagens.

*USAGE*

```shell
stegim steg [OPÇÕES] <img.jpg> [img0.jpg ... ] ] [texto.txt]
stegim steg --foreach-img-txt <img.jpg> <texto.txt> [img.jpg texto.txt ...]
stegim steg --nleast-significant-bit N <img.jpg> [img0.jpg ... ] ] [texto.txt]
stegim steg --channels CHANNEL_STRING [img0.jpg ... ] ] [texto.txt]
```

*CHANEL_STRING* String no formato [B-][G-][R-], i.e., uma string que habilita ou desabilita o canal.

Exemplos:

- `BGR` - Usa os 3 canais

- `B-R` - Usa somente o canal B e R

- `-G-` - Usa só o canal verde

- `---` - Não usa nenhum canal (errado!)

*OPÇÕES*

`-F` ou `--foreach-img-txt` Para cada imagem, esconde um .txt

`-b` ou `--nleast-significant-bit` Seta o número de bits menos significativos que serão usados na esteganografia.

`-C` ou `--channels` Altera os canais quer serão usados.

### info
--------

Função que irá exibir informações interessantes e úteis - no contexto da esteganografia - sobre a imagem. Exemplos de informações que o comando info vai dar:

* Número de máximo bytes que a imagem consegue esteganografar;

* Resolução da imagem;

*USAGE*

```shell
stegim info --nleast-significant-bit N <img.jpg> [img0.jpg ...]
```

*OPÇÕES*

`-b` ou `--nleast-significant-bit` Seta o número de bits menos significativos que serão usados na esteganografia.

`-C` ou `--channels` Altera os canais quer serão usados.

### x
----

Irá extrair o texto escondido da imagem.

```shell
stegim x [OPÇÕES] <img.jpg> [img1.jpg ... ]
stegim x -O saida.txt <img.jpg>
```


*OPÇÕES*

`-b` ou `--nleast-significant-bit` Seta o número de bits menos significativos que serão usados na esteganografia.

`-C` ou `--channels` Altera os canais quer serão usados.
