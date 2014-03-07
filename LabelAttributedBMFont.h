/*
 * RPG風文字送り＆ページ送りをするラベル
 *
 * 検証バージョン cocos2d-x v3.0 beta2
 *
 * 更新履歴
 * 2014.03.07  DisplaySpeedプロパティに0を設定した場合は一括表示するようにしました
 * 2014.03.06  追加
 * 
 */

#include "cocos2d.h"

struct KeywordDataSet {
    std::string word;
    cocos2d::Color3B strongColor;
    float scaleTo;
    std::vector < std::vector <unsigned int> > matchList;
    void addMatchList( std::vector< unsigned int > var) {
        matchList.push_back(var);
    };
};

class LabelAttributedBMFont : public cocos2d::LabelBMFont
{
    
public:
    LabelAttributedBMFont();
    virtual ~LabelAttributedBMFont();
    
    // 何フレーム周期で文字送りをするか
    CC_SYNTHESIZE(unsigned int, m_dispCycle, DispCycle);
    
    // 文字送りする際は何文字ずつ行うか (0を設定した場合は常に一括表示に。ただしページ送りやキーワード強調は有効)
    CC_SYNTHESIZE(unsigned int, m_dispSpeed, DispSpeed)
    
    // 文字送りが完了状態かどうか
    CC_SYNTHESIZE(bool, m_isAllCharDisplayed, IsAllCharDisplayed);
    
    /**
     文字色、文字サイズを局所的に変更したいキーワードを追加する
     @param keyword   キーワード
     @param fontColor キーワードの色
     @param fontScale キーワードのサイズ
     */
    void addKeyWord(std::string keyword, cocos2d::Color3B fontColor, float fontScale);
    
    /**
     キーワードを削除する
     @param 削除したいキーワード
     */
    void removeKeyWord(std::string keyword);
    
    /**
     文字送り中の文字を全て表示させる
     */
    void dispAllCharacters();
    
    /**
     ラベルの更新
     */
    virtual void updateLabel();
    
    virtual void update(float dt);

    static LabelAttributedBMFont * create(const char *str, const char *fntFile, float width, cocos2d::TextHAlignment alignment, cocos2d::Point imageOffset);
	static LabelAttributedBMFont * create(const char *str, const char *fntFile, float width, cocos2d::TextHAlignment alignment);
	static LabelAttributedBMFont * create(const char *str, const char *fntFile, float width);
	static LabelAttributedBMFont * create(const char *str, const char *fntFile);
    static LabelAttributedBMFont * create(std::vector< std::string > &pages, const char *fntFile);
    static LabelAttributedBMFont * create(std::vector< std::string > &pages, const char *fntFile, float width, cocos2d::TextHAlignment alignment, cocos2d::Point imageOffset);
    
    virtual bool initWithPages(const std::vector< std::string > &pages, const std::string& fntFile, float width = cocos2d::kLabelAutomaticWidth, cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::LEFT, cocos2d::Point imageOffset = cocos2d::Point::ZERO);
    void start();       // 文字送りスタート
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void setPages(std::vector< std::string > &pages);
    void setCallback( const std::function<void(cocos2d::Object*)> &callback );
    
protected:
    
    std::vector< std::string > m_pages;
    std::vector< std::string >::iterator m_iterator;
    cocos2d::ccMenuCallback m_callback;
    std::list< int > m_availableIndex;
    
private:
    
    std::list< KeywordDataSet > m_keyWords;

    /**
     キーワードの存在する位置を検出する
     */
    void searchKeywordsIndex();
    
    /**
     ラベル上に該当するキーワードがあればキーワードの文字色、文字サイズを変更する
     */
    void updateKeyWordsVisualInfo();
    
    /**
     ラベル上の文字色、文字サイズを初期化する
     */
    void initKeyWordsVisualInfo();
    
    int getStringLen();

};
