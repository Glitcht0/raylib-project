## ⚙️Glit Engine

Uma engine experimental desenvolvida em C++ utilizando [raylib](https://www.raylib.com/), criada para servir como base para projetos e experimentos de jogos e aplicações interativas.

> ⚠️ **Status:** Em desenvolvimento / fase de testes.
> A API e a organização interna ainda podem sofrer mudanças.

## Sobre a Engine

A engine utiliza o **raylib** como base para gerenciamento de janela, renderização, entrada, áudio e outros recursos gráficos.

A inicialização de cada projeto parte de um `main`, que configura e inicia a engine. A partir daí, o fluxo da aplicação é controlado por uma **máquina de estados integrada à engine**.

A ideia é permitir que diferentes partes da aplicação sejam isoladas em estados independentes, como:

- Menu principal
- Jogo
- Configurações
- Tela de carregamento
- Game Over
- Outros estados específicos do projeto

## ⛓️ Máquina de Estados

Cada estado da aplicação é baseado na classe `State`:

```cpp
class State {
protected:
    StateMachine* machine = nullptr;
    void ChangeState(State* next);

public:
    virtual ~State() = default;

    void SetMachine(StateMachine* m){
        machine = m;
    }

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update() = 0;
    virtual void draw() = 0;
};
```

Cada estado possui principalmente quatro pontos de interação:

### `onEnter()`

Executado quando o estado é iniciado.

É destinado à inicialização dos recursos específicos daquele estado, funcionando conceitualmente como um "segundo construtor".

### `onExit()`

Executado quando o estado é encerrado.

Pode ser utilizado para liberar recursos ou executar ações necessárias antes da saída do estado.

### `update()`

Responsável pela lógica do estado.

### `draw()`

Responsável pela renderização do estado.

### Mudança de estado

A troca de estados pode ser realizada diretamente através da máquina de estados:

```plaintext
GetStateMachine().ChangeState(new StateMenu());

```

Isso permite que cada parte da aplicação mantenha sua própria lógica sem concentrar todo o fluxo no `main`.

## Recursos

A engine atualmente possui ou está integrando diferentes ferramentas e sistemas:

### Raylib

Utilizado como base para os recursos gráficos e de interação da engine.

### RayGUI

Integração com o **RayGUI** para criação de interfaces gráficas.

### ASIO

Servidor baseado em **ASIO**, utilizado para recursos de comunicação em rede.

### Internacionalização (i18n)

Sistema de tradução baseado em **chaves JSON**, permitindo que textos sejam identificados por chaves em vez de ficarem diretamente espalhados pelo código.

Exemplo conceitual:

```plaintext
{
    "menu.play": "Jogar",
    "menu.settings": "Configurações",
    "menu.exit": "Sair"
}

```

Isso permite adicionar diferentes idiomas sem precisar modificar diretamente a lógica da aplicação.

### Fontes

Fontes gratuitas dentro da engine.

### Shaders

Suporte para arquivos de shaders simples.

### Outros recursos

Existem outras ferramentas e componentes experimentais presentes na engine, porém algumas delas ainda não estão suficientemente organizadas para serem consideradas parte de uma API estável.

Esses componentes poderão ser reorganizados ou modificados conforme o desenvolvimento avançar.

## Estrutura do projeto

A estrutura atual é aproximadamente:

```plaintext
.
├── Engine/
│   ├── ...
│
├── Projects/
│   ├── RoboCup/
│   │   ├── ...
│   │
│   └── ...
│
├── Libs/
│   ├── Asio/
│   ├── Json
│   ├── Raygui
│   └── ...
│
├── Makefile
└── ...

```

### `Engine/`

Contém o código da engine e seus sistemas compartilhados.

### `Projects/`

Contém os projetos desenvolvidos utilizando a engine.

Os projetos são baseados em um **template de projeto**, responsável por fornecer uma estrutura inicial para desenvolvimento.

A documentação do template será adicionada posteriormente.

## Compilação

Atualmente, a compilação é realizada através de um `Makefile`.

Como ainda não existe um sistema de criação automática de projetos, é necessário configurar manualmente quais arquivos `.cpp` pertencem ao projeto que será compilado.

Por exemplo:

```plaintext
# Pega todos os .cpp do projeto RoboCup
ALL_ROBOCUP_SRC = $(call rwildcard,Projects/RoboCup,*.cpp)

# Todas as fontes do projeto, incluindo subpastas
SRC = $(call rwildcard,Engine/,*.cpp) \
      $(filter-out Projects/RoboCup/main/%, $(ALL_ROBOCUP_SRC))

```

Nesse exemplo, a engine é compilada junto com os arquivos do projeto `RoboCup`, enquanto o `main` específico do projeto é excluído da busca automática.

Para adicionar outro projeto, atualmente é necessário alterar o `Makefile` manualmente, por exemplo:

```plaintext
#$(call rwildcard,Projects/TheGame,*.cpp)

```

Esse processo ainda é provisório.

### Próximos passos

Entre os objetivos futuros estão:

- [ ] Criar um sistema de criação rápida de projetos
- [ ] Melhorar o template de projetos
- [ ] Organizar os módulos experimentais da engine
- [ ] Melhorar a documentação
- [ ] Simplificar a configuração do `Makefile`
- [ ] Definir uma API mais estável para os sistemas da engine
- [ ] Adicionar mais ferramentas reutilizáveis
