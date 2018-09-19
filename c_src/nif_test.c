#include "erl_nif.h"
#include <stdio.h>

static ErlNifResourceType *fn1_res_type;

typedef struct {
    char buf1[1024 * 1024 * 1024];
    char buf2[1024 * 1024 * 512];
} fn1_res;

static ERL_NIF_TERM make_resource(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    printf("%s:%d allocating %d\r\n", __FUNCTION__, __LINE__, sizeof(fn1_res));
    fn1_res *res = (fn1_res*)enif_alloc_resource(fn1_res_type, sizeof(fn1_res));
    if(!res) {
        printf("%s:%d\r\n", __FUNCTION__, __LINE__);
        return enif_make_badarg(env);
    }
    
    ERL_NIF_TERM nif_res = enif_make_resource(env, res);
    enif_release_resource(res);
    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
    return nif_res;
}

static ERL_NIF_TERM clear_resource(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    fn1_res *res;
    int i;

    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
    if(!(argc == 1 &&
        enif_get_resource(env, argv[0], fn1_res_type, (void**)&res) )) {
        printf("%s:%d\r\n", __FUNCTION__, __LINE__);
        return enif_make_badarg(env);
    }
    printf("%s:%d allocating buf1 %d bytes\r\n", __FUNCTION__, __LINE__, sizeof(res->buf1));
    for(i = 0; i < sizeof(res->buf1); ++i)
        res->buf1[i] = 0x00;
    printf("%s:%d allocating buf2 %d bytes\r\n", __FUNCTION__, __LINE__, sizeof(res->buf2));
    for(i = 0; i < sizeof(res->buf2); ++i)
        res->buf2[i] = 0x00;

    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
    return enif_make_atom(env, "ok");
}

static void fn1_resource_dtor(ErlNifEnv *env, void *resource)
{
    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
}

// l(nif_test).
static int load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info)
{
    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
    fn1_res_type = enif_open_resource_type(env, NULL, "fn1_resource",
        fn1_resource_dtor, ERL_NIF_RT_CREATE, NULL);

    return 0;
}

// l(nif_test).
// l(nif_test).
static int upgrade(ErlNifEnv* env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info)
{
    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
    return 0;
}

// l(nif_test).
// l(nif_test).
// l(nif_test).
static void unload(ErlNifEnv* env, void* priv_data)
{
    printf("%s:%d\r\n", __FUNCTION__, __LINE__);
}

static ErlNifFunc nif_funcs[] =
{
    {"make_resource", 0, make_resource, ERL_NIF_DIRTY_JOB_IO_BOUND},
    {"clear_resource", 1, clear_resource, ERL_NIF_DIRTY_JOB_IO_BOUND}
};

ERL_NIF_INIT(nif_test, nif_funcs, load, NULL, upgrade, unload)
