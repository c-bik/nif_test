-module(intf).

-export([unload/0, free/0, init/0]).

init() ->
    case code:is_loaded(nif_test) of
        false -> code:load_file(nif_test);
        _ -> ok
    end,
    R = nif_test:make_resource(),
    nif_test:clear_resource(R),
    {mem_free, free(), '%'}.

unload() ->
    code:purge(nif_test),
    code:delete(nif_test),
    code:purge(nif_test),
    code:delete(nif_test).

free() ->
    #{total := Total, binary := Bin} = maps:from_list(erlang:memory()),
    ((Total - Bin) / Total) * 100.
