# Coding and File Structure Conventions

## Naming Conventions

-   **Aliases**\
    Alias names start with an uppercase letter.

-   **Global Variables**\
    Global variables are prefixed with an underscore (`_variableName`).\
    (Note: avoid `_X` where `X` is uppercase at global scope, as it may
    conflict with reserved identifiers.)

-   **Preprocessor Constants**\
    Preprocessor constants are written in full uppercase
    (`MY_CONSTANT`).

-   **Concepts (type constraints)**\
    Concepts are named with a `_type` suffix\
    (e.g., `Sortable_type`, `Numeric_type`).

## File Naming and Structure

-   **`.h` --- General Header**\
    Contains:

    -   Preprocessor directives (`#include`, macro constants, etc.)\
    -   Meta-programming utilities (aliases, concepts, traits, global
        variables)

-   **`.hpp` --- Class Definitions**\
    Contains class definitions.\
    If a class does not have its own `.hpp`, its definition is placed in
    the corresponding `.h`.

-   **`.tcc` --- Generic Template Implementations**\
    Contains non-specialized template implementations.

-   **`.ipp` --- Template Specialization Implementations**\
    Contains implementations of template specializations.

-   **`.cpp` --- Non-Template Implementations**\
    Contains implementations of classes or functions that do *not*
    require templates.
    
## Commenting Rules

-   **Definition-Oriented and definitives Comments (English)**\
    All comments that explain the meaning, purpose, behavior, or design
    of code elements\
    (e.g., class descriptions, function roles, algorithm explanations)\
    must be written in **English**.\
    This ensures clarity and universal readability.

-   **Temporary Work-In-Progress Comments (French)**\
    Comments related to ongoing development, such as notes and
    reminders\
    (e.g., "il faudra tester cette partie", "cette boucle pourrait être
    optimisée")\
    are written in **French**.\
    These comments are expected to be removed before finalization.
