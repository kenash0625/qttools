#ifndef LH_EL_CDATA_H
#define LH_EL_CDATA_H

#include "html_tag.h"

namespace litehtml
{
	class el_cdata : public element
	{
		tstring	m_text;
	public:
		el_cdata(const std::shared_ptr<litehtml::document>& doc);
		virtual ~el_cdata();

		virtual void	get_text(tstring& text) override;
		virtual void	set_data(const tchar_t* data) override;
	};
}

#endif  // LH_EL_CDATA_H
