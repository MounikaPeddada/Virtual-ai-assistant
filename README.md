# C++ Virtual Assistant (OOP Project)

A terminal-based virtual assistant built to demonstrate core OOP concepts
in C++: encapsulation, inheritance, polymorphism, and abstraction. No
LLM or external API is used — "intelligence" comes from your own C++
logic (keyword matching + rule-based grammar correction).

## Setup

You need `libsqlite3-dev` installed (SQLite was chosen over MySQL because
it needs no server — just one file — which makes the project trivial to
build and demo anywhere).

```bash
# Ubuntu/Debian/WSL
sudo apt-get install libsqlite3-dev

# macOS (Homebrew)
brew install sqlite3
```

## Build & Run

```bash
make        # compiles everything into ./assistant
make run    # compiles and runs
make clean  # removes binary and generated data files
```

Or manually:
```bash
g++ -std=c++17 -Iinclude src/*.cpp -o assistant -lsqlite3
./assistant
```

## Try it out

Once running, type things like:
```
hi
note i dont have time for teh meeting     -> grammar-corrected & saved
notes                                     -> lists saved notes
task finish DBMS assignment
tasks
done 1
add 5 and 12
what is your name
help
quit
```

## Architecture (where the OOP lives)

| Class | OOP concept | Role |
|---|---|---|
| `Command` (abstract) | **Abstraction / Polymorphism** | Base type for every action; `Assistant` calls `execute()` through a base-class pointer without knowing the concrete type |
| `GreetingCommand`, `TimeCommand`, `CalculatorCommand`, `KnowledgeCommand`, `HelpCommand`, `QuitCommand` | **Inheritance** | Concrete `Command` subclasses, each owning one behavior |
| `NoteCommand` | Inheritance | `Command` subclass wrapping `NoteManager` |
| `TaskCommand` | Inheritance | `Command` subclass wrapping `Database` |
| `TextProcessor` (abstract) | **Abstraction** | Interface for anything that transforms text |
| `GrammarCorrector` | Inheritance / Polymorphism | Rule-based `TextProcessor` implementation. Swap in an LLM-backed processor later with zero changes elsewhere — that's the point of coding to the interface |
| `NoteManager` | **Encapsulation** | Owns note storage (plain text file) behind `addNote()` / `viewNotes()` / `deleteNote()` |
| `Database` | **Encapsulation / Abstraction** | Wraps all SQL (tasks, conversation history, preferences) — nothing outside this class writes raw SQL |
| `Task` | **Encapsulation / Friend function** | Private `id`/`description`/`done` with getters; `operator<<` is a `friend` (can't be a member since `ostream` must be the left operand, but still needs private access) |
| `User` | Encapsulation | Private `name` with getter/setter |
| `Assistant` | Composition | Owns a `User`, `Database&`, `NoteManager&`, and a `vector<unique_ptr<Command>>`; dispatches input to the first matching command |

### Storage design choice
- **Notes** → flat text file (`notes.txt`) — free text, no need for structured queries, so a file is the leaner choice.
- **Tasks / conversation history / preferences** → SQLite (`assistant.db`) — these benefit from structured queries (marking done, filtering, timestamps).

### Data flow for a note
```
user input -> NoteCommand -> NoteManager.addNote(text)
                                   |
                                   v
                         TextProcessor::process(text)   (interface)
                                   |
                                   v
                     GrammarCorrector::process(text)     (concrete impl)
                                   |
                                   v
                         corrected text written to notes.txt
```

## Extending later
- Swap `GrammarCorrector` for a real LLM-backed `TextProcessor` once you've
  covered APIs — `NoteManager` never needs to change, since it only depends
  on the `TextProcessor` interface (Strategy pattern / open-closed principle).
- Add more `Command` subclasses (e.g. `WeatherCommand`, `PersonaCommand`) by
  just implementing `matches()` / `execute()` / `description()` and
  registering them in `Assistant::registerCommands()`.

## File layout
```
include/    all class headers (.h)
src/        all implementations (.cpp), including main.cpp
Makefile
README.md
```
