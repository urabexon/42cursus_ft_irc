/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmmandUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: urabex <urabex@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 01:10:43 by urabex            #+#    #+#             */
/*   Updated: 2025/05/21 14:49:25 by urabex           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

std::vector<std::string> splitMessage(std::string message) {
	std::istringstream iss(message);
	std::vector<std::string> words;
	std::string word;

	while (iss >> word) {
		trim(word);
		words.push_back(word);
	}
	return words;
}

std::string getChannelNameFromWord(std::string &word) {
	if (word[0] == '#')
		return (word.substr(1));
	return (word);
}

void getTargetAndText(std::string &argument, std::string &target, std::string &text) {
	size_t pos = argument.find(' ');
	if (pos == std::string::npos) {
		target = getChannelNameFromWord(argument);
		text = "";
	} else {
		target = argument.substr(0, pos);
		target = getChannelNameFromWord(target);
		text = argument.substr(pos + 1);
		if (!text.empty() && text[0] != ':')
			text = ":" + text;
	}
}
