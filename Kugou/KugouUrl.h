#ifndef KUGOUURL_H
#define KUGOUURL_H

class KugouUrl
{
public:
    KugouUrl();

    // 乐库
    static QString getYuekuUrl();
    static QString getRecommendUrl();   // 推荐
    static QString getRankUrl();        // 排行榜
    static QString getSingerUrl();      // 歌手
    static QString getCategoryUrl();    // 分类
    static QString getShowUrl();        // 节目
};

#endif // KUGOUURL_H
