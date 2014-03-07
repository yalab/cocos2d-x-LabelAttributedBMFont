#include "LabelAttributedBMFont.h"

USING_NS_CC;
using namespace std;

#define NEWLINE_CHAR    "\n"
#define NULL_CHAR       ""


LabelAttributedBMFont::LabelAttributedBMFont()
{
}

LabelAttributedBMFont::~LabelAttributedBMFont()
{
    log("LabelAttributedBMFont ~LabelAttributedBMFont()");
}

LabelAttributedBMFont* LabelAttributedBMFont::create(const char *str, const char *fntFile, float width, TextHAlignment alignment)
{
	return LabelAttributedBMFont::create(str, fntFile, width, alignment, Point::ZERO);
}

LabelAttributedBMFont* LabelAttributedBMFont::create(const char *str, const char *fntFile, float width)
{
    return LabelAttributedBMFont::create(str, fntFile, width, TextHAlignment::LEFT, Point::ZERO);
}

LabelAttributedBMFont* LabelAttributedBMFont::create(const char *str, const char *fntFile)
{
    return LabelAttributedBMFont::create(str, fntFile, kCCLabelAutomaticWidth, TextHAlignment::LEFT, Point::ZERO);
}

LabelAttributedBMFont* LabelAttributedBMFont::create(const char *str, const char *fntFile, float width, cocos2d::TextHAlignment alignment, cocos2d::Point imageOffset)
{
    vector< string > pages;
    pages.push_back(str);
    return LabelAttributedBMFont::create(pages, fntFile, width, alignment, imageOffset);
}

LabelAttributedBMFont* LabelAttributedBMFont::create(std::vector<std::string> &pages, const char *fntFile)
{
    return LabelAttributedBMFont::create(pages, fntFile, kCCLabelAutomaticWidth, TextHAlignment::LEFT, Point::ZERO);
}

LabelAttributedBMFont* LabelAttributedBMFont::create(std::vector<std::string> &pages, const char *fntFile, float width, cocos2d::TextHAlignment alignment, cocos2d::Point imageOffset)
{
    LabelAttributedBMFont* pRet = new LabelAttributedBMFont();
    
    if (pRet && pRet->initWithPages(pages, fntFile, width, alignment, imageOffset)) {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return NULL;
}

bool LabelAttributedBMFont::initWithPages(const std::vector< std::string > &pages, const std::string& fntFile, float width, cocos2d::TextHAlignment alignment, cocos2d::Point imageOffset)
{
    for (int i = 0; i < pages.size(); i++) {
        m_pages.push_back(pages[i]);
    }
    m_iterator = m_pages.begin();
    
    if (! LabelBMFont::initWithString(*m_iterator, fntFile, width, alignment, imageOffset)) {
        return false;
    }
    
	m_dispSpeed = 1;
	m_dispCycle = 5;
	m_isAllCharDisplayed = false;
    
    // touch event
    auto listner = EventListenerTouchOneByOne::create();
    listner->setSwallowTouches(false);
    listner->onTouchBegan = CC_CALLBACK_2(LabelAttributedBMFont::onTouchBegan, this);
    listner->onTouchEnded = CC_CALLBACK_2(LabelAttributedBMFont::onTouchEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listner, this);
    
    return true;
    
}

void LabelAttributedBMFont::start()
{
    if (m_dispSpeed == 0) {
        dispAllCharacters();
        return;
    }
    scheduleUpdate();
}

void LabelAttributedBMFont::setPages(std::vector<std::string> &pages)
{
    m_pages.clear();
    for (int i = 0; i < pages.size(); i++) {
        m_pages.push_back(pages[i]);
    }
    m_iterator = m_pages.begin();
    setString(*m_iterator, true);
    start();
}

void LabelAttributedBMFont::setCallback(const std::function<void (cocos2d::Object *)> &callback)
{
    m_callback = callback;
}

void LabelAttributedBMFont::setCallbackChangedPage(const std::function<void (int)> &callback)
{
    m_callbackChangedPage = callback;
}

bool LabelAttributedBMFont::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void LabelAttributedBMFont::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    // 全ページ表示後なら
    if (m_iterator == m_pages.end()) {
        log("LabelAttributedBMFont : fin");
        return;
    }
    
    // 文字送りがおわってなかったら
    if (! this->getIsAllCharDisplayed()) {
        this->dispAllCharacters();
        return;
    }
    
    m_iterator++;
    if (m_iterator == m_pages.end()) {
        log("LabelAttributedBMFont : callback");
        if (m_callback) m_callback(this);
        return;
    }
    setString(*m_iterator, true);
    
    if (m_callbackChangedPage) {
        auto dif = m_iterator - m_pages.begin();
        m_callbackChangedPage(dif);
    }
    
    start();
}


/**
 引数の文字列のマルチバイトを考慮した文字長を取得する
 @param str 対象文字列
 @return 文字長
 */
unsigned int getMuitiByteStringLength(const char *str)
{
	string targetStr = string(str);
	int len = 0;
	for (int i = 0; i < targetStr.length(); ) {
		unsigned char code = str[i];
		((code >= 0x81 && code <= 0x9F) || (code >= 0xe0 && code <= 0xfc)) ? i += 3 : i++;
		len++;
	}
	return len;
}

/**
 文字色、文字サイズを局所的に変更したいキーワードを追加する
 @param keyword   キーワード
 @param fontColor キーワードの色
 @param fontScale キーワードのサイズ
 */
void LabelAttributedBMFont::addKeyWord(string keyword, Color3B fontColor, float fontScale)
{
	// 改行コードの場合は何もしない
	if (keyword == NEWLINE_CHAR) {
		log("LabelAttributedBMFont addKeyWord() keyword is newline character!!!");
		return;
	}
    
	KeywordDataSet dataset;
    dataset.word = keyword;
    dataset.strongColor = fontColor;
    dataset.scaleTo = fontScale;
    dataset.matchList.clear();
    m_keyWords.push_back(dataset);
}

/**
 キーワードを削除する
 @param 削除したいキーワード
 */
void LabelAttributedBMFont::removeKeyWord(string keyword)
{
    auto it = m_keyWords.begin();
    while (it != m_keyWords.end()) {
        auto &dataset = *it;
        if (dataset.word != keyword) {
            it++;
            continue;
        }
        m_keyWords.erase(it);
        return;
    }
    log("LabelAttributedBMFont not found keyword:%s", keyword.c_str());
	
}

/**
 文字列の置換
 @param targetStr 置換をかける文字列
 @param fromStr   置換対象文字列
 @param toStr     置換後文字列
 @return          置換完了後文字列
 */
string replace(string &targetStr, string fromStr, string toStr)
{
	string::size_type pos = targetStr.find_first_of(fromStr, 0);
	while (string::npos != pos) {
		targetStr.replace(pos, string(fromStr).length(), toStr);
		pos = targetStr.find_first_of(fromStr, pos);
	}
	return targetStr;
}

/**
 ラベル上の文字色、文字サイズを初期化する
 */
void LabelAttributedBMFont::initKeyWordsVisualInfo()
{
    m_availableIndex.clear();
    log("LabelAttributedBMFont : initKeyWordsVisualInfo()");
    auto children = getChildren();
    for (int i = 0; i < children.size(); i++) {
        auto *mChar = (Sprite *)children.at(i);
        
        // ここで表示設定なのか非表示設定なのか判別しておく
        // そしてちゃんと扱うべきtagを保持。
        // じゃないと、文字数とか改行の数とか、内部で保持しているSpreiteの数とかで齟齬が生じる
        // 文字数＝Spriteの数ではないので！
        
        if (! mChar->isVisible()) {
            continue;
        }
        
        m_availableIndex.push_back(mChar->getTag());      // 扱うべきtagを保持する！
        mChar->setColor(Color3B::WHITE);
		mChar->setScale(1.0f);
		mChar->setVisible(false);
        
    }
    
    auto it = m_keyWords.begin();
    while (it != m_keyWords.end()) {
        auto &dataset = *it;
        dataset.matchList.clear();
        it++;
    }
    
    m_availableIndex.sort();
    m_isAllCharDisplayed = false;
}

/**
 文字送り中の文字を全て表示させる
 */
void LabelAttributedBMFont::dispAllCharacters()
{
    if (m_isAllCharDisplayed) {
        return;
    }
    m_isAllCharDisplayed = true;
    unscheduleUpdate();
    
    auto it = m_availableIndex.begin();
    // 扱うべきidxリストから抽出して表示すること
    // 考えずに全部表示すると、前回表示時に残ってるSpriteが表示されたりする
    while (it != m_availableIndex.end()) {
        auto *mChar = getChildByTag(*it);
        mChar->setVisible(true);
        it++;
    }
}

/**
 キーワードの存在する位置を検出する
 */
void LabelAttributedBMFont::searchKeywordsIndex()
{
	// ラベル上に表示している文字列
	string newStr = string(_initialStringUTF8);
    
	// 改行コードを除去
	newStr = replace(newStr, NEWLINE_CHAR, NULL_CHAR);
    
	// 文字色、文字サイズを初期化
	initKeyWordsVisualInfo();

    auto it = m_keyWords.begin();
	// 登録したキーワード分だけ検索処理を行う
    while (it != m_keyWords.end()) {
		auto &dataset = *it;
        auto key = dataset.word;
		unsigned int keyLengthMultiByte = getMuitiByteStringLength(key.c_str());     // キーワードの長さ(マルチバイト)
		unsigned int searchIndex = 0;                                       // 現在の検出走査位置
		unsigned int keyFindIndex = newStr.find(key, searchIndex);          // キーワードが検出された位置

		while (keyFindIndex != string::npos) {
			unsigned int keyFindIndexMultiByte = 0;                         // キーワードが検出された位置(マルチバイト)
			// キーワードが検出された位置(マルチバイト)を検出
			string clipStr = newStr.substr(0, keyFindIndex);
			keyFindIndexMultiByte = getMuitiByteStringLength(clipStr.c_str());
            
            vector< unsigned int > list;
            list.push_back(keyFindIndexMultiByte);  // 開始位置
            list.push_back(keyLengthMultiByte);     // その長さ
            dataset.addMatchList(list);
            
			// 現在の検出走査位置をキーワード終了位置へ
			searchIndex = (keyFindIndex + key.length());
            
			// キーワード終了位置以降に存在するキーワードを検出
			keyFindIndex = newStr.find(key, searchIndex);
		}
        
        it++;
	}
}


/**
 ラベル上に該当するキーワードがあればキーワードの文字色、文字サイズを変更する
 */
void LabelAttributedBMFont::updateKeyWordsVisualInfo()
{
    auto it = m_keyWords.begin();
    
    while (it != m_keyWords.end()) {
        auto &dataset = *it;
        auto it2 = dataset.matchList.begin();
        while (it2 != dataset.matchList.end()) {
            auto range = *it2;
            int startIndex = range.at(0);
            int length = range.at(1);
            auto it3 = m_availableIndex.begin();
            advance(it3, startIndex);
            do {
                auto mChar = (Sprite *)getChildByTag(*it3);
                mChar->setColor(dataset.strongColor);
                mChar->setScale(dataset.scaleTo);
                
                it3++;
                length--;
            } while (length > 0 && it3 != m_availableIndex.end());
            
            it2++;
        }
        
        it++;
    }
}

/**
 ラベルの更新
 */
void LabelAttributedBMFont::updateLabel()
{
    
	LabelBMFont::updateLabel();
    
	// キーワードの存在する位置を検出する
	searchKeywordsIndex();
    
	// キーワードの文字色、文字サイズを更新
	updateKeyWordsVisualInfo();
}

/**
 更新、文字送りを行う
 @param dt 前回コールされてからの経過時間
 */
void LabelAttributedBMFont::update(float dt)
{
    if (m_availableIndex.size() == 0) {
        unscheduleUpdate();
        return;
    }

	if (Director::getInstance()->getTotalFrames() % m_dispCycle == 0) {
        
		for (int i = 0; i < m_dispSpeed; i++) {
            
            auto tag = m_availableIndex.front();
            m_availableIndex.pop_front();
            Sprite *sprite = (Sprite *)getChildByTag(tag);
            sprite->setVisible(true);

		}
        
        if (m_availableIndex.size() == 0) {
            m_isAllCharDisplayed = true;
            unscheduleUpdate();
        }

	}
}


int LabelAttributedBMFont::getStringLen()
{
    return _string ? cc_wcslen(_string) : 0;
}

