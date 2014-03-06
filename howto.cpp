// 簡単に使い方

// 表示文字準備
vector< string > pages;
pages.push_back("あいうえおー\nかきくけこー");
pages.push_back("さしすせそー");

auto label = LabelAttributedBMFont::create(lines, "hoge.fnt");

/*
普通に文字列でもok
auto label = LabelAttributedBMFont::create("ページ送りはないよー\n1ページだけだよー", "hoge.fnt");
*/

// 強調キーワードを設定する場合
label->addKeyWord("おー", Color3B::RED, 1.f);   // 強調キーワード, 強調色, 文字スケール
// 複数あるならもう繰り返し
label->addKeyWord("すせー", Color3B::YELLOW, 1.f);   // 強調キーワード, 強調色, 文字スケール

// コールバック設定するなら(全表示が終わったときにタップされると呼ばれる)
label->setCallback([](Object *sender){
  // 処理記述
});

// 文字送り開始(これやんないと何も始まらないYo)
label->start();
