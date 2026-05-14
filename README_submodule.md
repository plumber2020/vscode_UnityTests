## TestSubmoduleCpp
Branch: feature/sum

## СПРАВКА
https://docs.gitflic.ru/latest/project/submodule/

### Как добавить Git submodule
Добавляем git submodule add <submodule_url> <path> 
- аргумент -b -branch-: следим за основной веткой репозитория -branch-
- аргумент <path>     : путь, по которому необходимо добавить подмодуль в репозиторий
```
git submodule add -b -branch- https://github.com/plumber2020/TestSubmoduleCpp.git
```
### Как клонировать репозиторий с подмодулями Git
- инициализируем подмодули, определённые в репозитории
```
git submodule init
```
- извлекаем последние коммиты из репозиториев подмодулей
```
git submodule update
```
### Обновление подмодулей
- Автоматически (на последний коммит ветки)
```
git submodule update --remote --recursive
```
- Вручную (на конкретный коммит или тег)
```
cd path/to/submodule
git fetch
git checkout <commit-or-tag>
```
### Удаление подмодулей
- Автоматически
```
git submodule deinit -f path/to/submodule
git rm -f path/to/submodule
```