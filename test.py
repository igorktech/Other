import config
import telebot, wikipedia, re
# Create bot
bot = telebot.TeleBot(token=config.TOKEN)
# Choose language Wikipedia
wikipedia.set_lang("ru")
# Clear text from Wikipedia
def getwiki(s):
    try:
        ny = wikipedia.page(s)
        # Get first 1000 symbols
        wikitext=ny.content[:1000]
        # Split points
        wikimas=wikitext.split('.')
        # delete all after last point
        wikimas = wikimas[:-1]
        # Result
        wikitext2 = ''
        # All strings except title
        for x in wikimas:
            if not('==' in x):
                # if in string there are more than 3 symbols add to result
                if(len((x.strip()))>3):
                   wikitext2=wikitext2+x+'.'
            else:
                break
        # Text filtering with regular expressions
        #wikitext2=re.sub('\([^()]*\)', '', wikitext2)
        #wikitext2=re.sub('\([^()]*\)', '', wikitext2)
        #wikitext2=re.sub('\{[^\{\}]*\}', '', wikitext2)
        # Return string
        return wikitext2
    # Except wikipedia
    except Exception as e:
        return 'В энциклопедии нет информации об этом'
# Command handler /start
@bot.message_handler(commands=["start"])
def start(m, res=False):
    bot.send_message(m.chat.id, 'Отправьте мне любое слово, и я найду его значение на Wikipedia')
# get message from user
@bot.message_handler(content_types=["text"])
def handle_text(message):
    bot.send_message(message.chat.id, getwiki(message.text))
# start polling
bot.polling(none_stop=True, interval=0)