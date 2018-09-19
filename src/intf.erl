-module(intf).

-export([unload/0, load/0, init/0]).

init() ->
    R = nif_test:make_resource(),
    nif_test:clear_resource(R).

unload() ->
    code:purge(nif_test),
    code:delete(nif_test),
    code:purge(nif_test),
    code:delete(nif_test).

load() ->
    code:load_file(nif_test),
    code:load_file(nif_test),
    code:load_file(nif_test).
