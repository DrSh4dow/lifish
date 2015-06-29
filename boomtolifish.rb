#!/usr/bin/ruby
# Convert BOOM json levels to lifish levels

require 'json'

unless ARGV.length > 0
	puts "Usage: #{$0} <boom_levels.json>"
	exit 1
end

file = File.read ARGV[0]
boom_levels = JSON.parse(file)['LevelDescription']

# A BOOM level's structure is:
# {
#	"FixedBlockID": num,
#	"BreakableBlockID": num,
#	"BorderID": num,
#	"BGPatternID": num,
#	"GridDescString: string,
#	"Time": num
# }
#
# We need to convert it to lifish format:
# {
#	"time": num,
#	"music": num,
#	"tileIDs": {
#		"bg": num,
#		"border": num,
#		"fixed": num,
#		"breakable": num
#	},
#	"tilemap": string
# }

lifish_hash = {
	"name" => "Converted BOOM levels",
	"author" => "Federico Filipponi",
	"difficulty" => "average",
	"tracks" => [
		{
			"loop" => {
				"start" => 7.386,
				"length" => 44.279
			}
		}
	],
	"levels" => []
}

boom_levels.each_with_index do |level, i|
	lv = {
		"time" => level["Time"],
		"music" => 1, #(i / 10).floor + 1,
		"tileIDs" => {
			"bg" => (i / 10).floor + 1,
			"border" => (i / 10).floor + 1,
			"fixed" => (i / 10).floor + 1,
			"breakable" => (i / 10).floor + 1
		},
		"tilemap" => level["GridDescString"]
	}
	lifish_hash["levels"] << lv
end

File.open("converted.json", "w") do |f|
	f.write lifish_hash.to_json
end
