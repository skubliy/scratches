# 1 "main.cpp"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "main.cpp"
# 21 "main.cpp"
int main() {

    int serverVsId = 1
const struct sockaddr *servAddr = (struct sockaddr *)&( ( ( int )(0) >= 0 && (0) < ((&( proxy_config.virtualServersMap[serverVsId]->pool )->nodes))->SL_VECTOR_VAR_count ? ((&( proxy_config.virtualServersMap[serverVsId]->pool )->nodes))->SL_VECTOR_VAR_array + (0) : NULL )->server ).addr;


    return 0;
}
