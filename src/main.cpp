#include <Geode/Geode.hpp>
#include <Geode/modify/CustomizeObjectLayer.hpp>

using namespace geode::prelude;

class $modify(MyCustomizeObjectLayer, CustomizeObjectLayer) {
	
	struct Fields {
		bool m_textObjects = false;
		CCLabelBMFont* m_warningLabel = nullptr;
	};

    bool init(GameObject* p0, cocos2d::CCArray* p1) {

		auto fields = m_fields.self();

		if (!p0) {
			bool areTextObjects = true;
			for (GameObject* object : CCArrayExt<GameObject*>(p1)) {
				if (object->m_objectID != 914) {
					areTextObjects = false;
					break;
				}
			}
			if (areTextObjects) {
				fields->m_textObjects = true;
				auto ret = CustomizeObjectLayer::init(static_cast<GameObject*>(p1->objectAtIndex(0)), CCArray::create());
				fields->m_warningLabel = CCLabelBMFont::create("This will override all selected text objects!", "bigFont.fnt");
				fields->m_warningLabel->setScale(0.4f);
				fields->m_warningLabel->setVisible(false);
				fields->m_warningLabel->setPosition(m_kerningLabel->getPosition());
				m_mainLayer->addChild(fields->m_warningLabel);
				m_targetObjects = p1;
				m_targetObject = nullptr;
				return ret;
			}
		}
		return CustomizeObjectLayer::init(p0, p1);
	};

    void onSelectMode(cocos2d::CCObject* sender) {
		auto fields = m_fields.self();

		CustomizeObjectLayer::onSelectMode(sender);
		m_kerningSlider->setVisible(false);
		m_kerningLabel->setVisible(false);
		if (fields->m_warningLabel) {
			fields->m_warningLabel->setVisible(sender->getTag() == 3);
		}
	}

	void onClear(CCObject* sender) {
		auto fields = m_fields.self();
		if (fields->m_textObjects) {
			for (TextGameObject* object : CCArrayExt<TextGameObject*>(m_targetObjects)) {
				object->updateTextObject("", true);
			}
			m_textInput->setString("");
			return;
		}
		CustomizeObjectLayer::onClear(sender);
	}

    void textChanged(CCTextInputNode* p0) {
		auto fields = m_fields.self();
		if (fields->m_textObjects) {
			for (TextGameObject* object : CCArrayExt<TextGameObject*>(m_targetObjects)) {
				object->updateTextObject(p0->getString(), true);
			}
			return;
		}
		CustomizeObjectLayer::textChanged(p0);
	}

    void onClose(cocos2d::CCObject* sender) {
		auto fields = m_fields.self();
		if (fields->m_textObjects) {
			m_targetObject = static_cast<GameObject*>(m_targetObjects->objectAtIndex(0));
			m_targetObjects = nullptr;
		}
		CustomizeObjectLayer::onClose(sender);
	}

};