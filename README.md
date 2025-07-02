# Sistema de Biblioteca em C

Este é um sistema de gerenciamento de biblioteca desenvolvido em linguagem C, com estrutura baseada em **tabela hash encadeada**. O projeto foi feito com fins educacionais, visando a prática de manipulação de estruturas de dados dinâmicas, como listas encadeadas, além de modularização e validação de entrada.

## Funcionalidades

- Cadastro de livros com título, autor, ano e gênero
- Geração e reaproveitamento automático de IDs
- Empréstimo e devolução de livros
- Remoção e busca por ID
- Listagem de todos os livros cadastrados
- Exibição da estrutura da tabela hash
- Limpeza completa da biblioteca

## Tecnologias utilizadas

- Linguagem C
- Biblioteca padrão (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `locale.h`)
- Tabela hash com tratamento de colisão via encadeamento